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

namespace csl
{
  namespace comm
  {
    /*
    socket_type_t      sock_;
    csl::common::str   hostname_;
    unsigned short     port_;
    std::thread        thread_;
    bool               started_;
    std::mutex         lock_;
    uint32_t           suspend_interval_;
    fdhandler *        handler_;
    */

    namespace tcp
    {
      namespace
      {
        typedef std::lock_guard<std::mutex> scoped_lock;
      }

      listener::listener(
                const common::str & hostname,
                unsigned short port,
                fdhandler & h,
                int backlog)
        :
          hostname_(hostname),
          port_(0),
          started_(false),
          stop_flag_(false),
          suspend_interval_(0),
          handler_(&h),
          backlog_(backlog) { }

      listener::~listener()
      {
        stop();
      }

      bool listener::start()
      {
        bool ret = false;
        {
          struct sockaddr_in address;

          // XXX TODO set address here !!!!

          scoped_lock lck(lock_);
          if( started_ )
          {
            CSL_THROW(already_started);
          }

          sock_ = ::socket( AF_INET, SOCK_STREAM, 0 );
          if( sock_.get() <= 0 )
          {
            CSL_THROW(failed_to_create_socket);
          }

          int on = 1;
          if( ::setsockopt( sock_.get(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) ) < 0 )
          {
            CSL_THROW(failed_to_set_reuseaddr);
          }

          if( ::bind( sock_.get(),
                      reinterpret_cast<const struct sockaddr *>(&address),
                      sizeof(address) ) < 0 )
          {
            CSL_THROW(failed_to_bind);
          }

          if( ::listen( sock_.get(), backlog_ ) < 0 )
          {
            CSL_THROW(listen_failed);
          }

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
          stop_flag_ = true;
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
            CSL_THROW( not_started );
          }
        }

        while( stop_flag_ == false )
        {
        }

        sock_      = -1;
        started_   = false;
        stop_flag_ = false;
      }
    }
  }
}

// EOF
