/*
Copyright (c) 2008,2009,2010,2011 CodeSLoop Team

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "codesloop/comm/tcp_listener.hh"
#include "codesloop/common/logger.hh"

namespace csl
{
  namespace comm
  {
    namespace tcp
    {
      namespace
      {
        typedef std::lock_guard<std::mutex> scoped_lock;

        class autofree_addrinfo
        {
        public:
          typedef struct addrinfo* addrinfo_ptr;
          autofree_addrinfo(addrinfo_ptr & p) : ptr_(&p) {}
          ~autofree_addrinfo() { if(*ptr_!=NULL) { freeaddrinfo(*ptr_); *ptr_ = NULL; } }
        private:
          addrinfo_ptr * ptr_;
          autofree_addrinfo() = delete;
        };
      }

      listener::listener(
                const std::string & hostname,
                const std::string & port,
                fdhandler & h,
                int backlog)
        :
          hostname_(hostname),
          port_(port),
          started_(false),
          stop_me_(false),
          suspend_interval_(0),
          handler_(&h),
          backlog_(backlog)
      {
      }

      listener::~listener()
      {
        stop();
      }

      bool listener::start()
      {
        bool ret = false;
        {
          scoped_lock lck(lock_);
          if( started_ )
          {
            CSL_THROW( already_started );
          }

          struct addrinfo hints, *result=NULL, *rp=NULL;
          autofree_addrinfo result_guard(result);

          memset(&hints,0,sizeof(struct addrinfo));
          hints.ai_family = AF_INET;
          hints.ai_socktype = SOCK_STREAM;

          int s = getaddrinfo(hostname_.c_str(),port_.c_str(),&hints,&result);
          if(s!=0)
          {
            CSL_THROW(failed_to_resolve_name);
          }

          for( rp=result; rp!=NULL; rp = rp->ai_next )
          {
            sock_ = ::socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );

            if( sock_.get() == -1 )
            {
              CSL_THROW(failed_to_create_socket);
            }

            int on = 1;
            if( ::setsockopt( sock_.get(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) < 0 )
            {
              CSL_THROW(failed_to_set_reuseaddr);
            }

            if( bind(sock_.get(),rp->ai_addr, rp->ai_addrlen) == 0 )
            {
              addr_.set( rp->ai_addr, rp->ai_addrlen );
              ret = true;
              break;
            }
            sock_.close();
          }

          if( sock_.get() == -1 )
          {
            CSL_THROW(failed_to_bind);
          }

          if( ::listen( sock_.get(), backlog_ ) < 0 )
          {
            CSL_THROW(listen_failed);
          }

          started_ = true;
          thread_ =  std::thread(&listener::loop,this);
        }

        return ret;
      }

      bool listener::stop()
      {
        bool ret = false;
        {
          scoped_lock lck(lock_);
          ret = started_;
          stop_me_ = true;
          if( ret ) thread_.join();
        }
        return ret;
      }

      void listener::loop()
      {
        if( !started_ )
        {
          scoped_lock lck(lock_);
          if( sock_.get() == -1 )
          {
            CSL_ERROR( L"Failed to create socket" );
            return;
          }
        }

        CSL_INFO( L"Starting TCP listener loop" );

        fd_set rfds;

        while( stop_me_ == false )
        {
          addr incoming_addr;
          struct timeval tv = { 1,0 };
          FD_ZERO(&rfds);
          FD_SET(sock_.get(),&rfds);

          int err = ::select(sock_.get()+1,&rfds,NULL,NULL,&tv);
          if( err == -1 )
          {
            // treat this as a permanent error because that usually means
            // the listener socket is in bad shape
            CSL_ERROR( L"select() failed" );
            break;
          }
          else if (err)
          {
            int fdtmp = ::accept(
                            sock_.get(),
                            incoming_addr.get(),
                            &incoming_addr.len());

            if( fdtmp > 0 )
            {
              try
              {
                (*handler_)(autofd(fdtmp),incoming_addr);
                suspend_interval_ = 0;
              }
              catch(const fdhandler::stop & e)
              {
                stop_me_ = true;
              }
              catch(const fdhandler::suspend & e)
              {
                suspend_interval_ += 100;
                SleepMiliseconds(suspend_interval_);
              }
            }
            else if( fdtmp < 0 )
            {
              // treat this as a transient error, because this can be caused by a
              // temporary inability of handling new connections
              // --> we give some time to the system to recover

              suspend_interval_ += 100;

              CSL_ERROR( L"accept() failed (sleeping for " << suspend_interval_ << L" ms)" );

              SleepMiliseconds(suspend_interval_);
            }
          }
        }

        sock_    = -1;
        started_ = false;
        stop_me_ = false;
      }
    }
  }
}

// EOF
