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

#include "codesloop/comm/udp_listener.hh"
#include "codesloop/comm/addr.hh"

namespace csl
{
  namespace comm
  {
    namespace udp
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
                msghandler & h)
        :
          hostname_(hostname),
          port_(port),
          started_(false),
          stop_me_(false),
          suspend_interval_(0),
          handler_(&h),
          buffer_(new buffer_t() ) { }

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
          hints.ai_socktype = SOCK_DGRAM;

          int s = getaddrinfo(hostname_.c_str(),port_.c_str(),&hints,&result);
          if(s!=0)
          {
            CSL_THROW(failed_to_resolve_name);
          }

          for( rp=result; rp!=NULL; rp = rp->ai_next )
          {
            sock_ = ::socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );

            if( sock_.get() <= 0 )
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
            // XXX ???
            CSL_THROW(not_started);
          }
        }

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
            // XXX ???
            CSL_THROW( select_failed );
          }
          else if (err)
          {
            // allocate read buffer
            typename buffer_t::result buf;
            buffer_->get(buf);
            // receive data
            ssize_t recvd = ::recvfrom(
                                sock_.get(),
                                buf.buf_,
                                buffer_t::buf_size_,
                                0, // flags
                                incoming_addr.get(),
                                &incoming_addr.len() );

            // ignore errors, assume select will signal socket and system related
            // problems
            if( recvd > 0 )
            {
              // being defensive: buffer cannot be larger than buf_size_
              // create the message
              common::ksmsg m(
                  common::ksmsg::buf(
                      buf.buf_,
                      (static_cast<size_t>(recvd) & (buffer_t::buf_mask_))),
                  buf.get_lock());

              try
              {
                // pass the message
                (*handler_)(m,incoming_addr,sock_.get());
                suspend_interval_ = 0;
              }
              catch(const msghandler::stop & e)
              {
                stop_me_ = true;
              }
              catch(const msghandler::suspend & e)
              {
                suspend_interval_ += 100;
                SleepMiliseconds(suspend_interval_);
              }
            }
            else if( recvd < 0 )
            {
              // XXX ???
              CSL_THROW( recvfrom_failed );
            }
          }
        }

        sock_      = -1;
        started_   = false;
        stop_me_   = false;
      }
    }
  }
}

// EOF
