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

#ifndef _csl_comm_tcp_listener_hh_included_
#define _csl_comm_tcp_listener_hh_included_
#include "codesloop/common/excbase.hh"
#include "codesloop/common/str.hh"
#include <thread>
#include <mutex>

namespace csl
{
  namespace comm
  {
    namespace tcp
    {
      class listener
      {
      public:
        typedef int socket_type_t;

        CSL_CLASS( csl::com::tcp::listener );
        CSL_DECLARE_EXCEPTION( already_started );

      public:
        listener(
            const csl::common::str & hostname,
            unsigned short port,
            handler & handler) {}

        ~listener() {}

        bool start() { return false; }
        bool stop() { return false; }

        class handler
        {
        public:
          CSL_CLASS( csl::com::tcp::listener );
          CSL_DECLARE_EXCEPTION( stop );
          CSL_DECLARE_EXCEPTION( suspend );

          virtual void operator()(listener::sock_type_t s) = 0;
          virtual ~handler() {}
        };

      private:
        listener() {}
        listener(const listener&) = delete;
        listener & operator=(const listener &) = delete;

        socket_type_t      sock_;
        csl::common::str   hostname_;
        unsigned short     port_;
        std::thread        thread_;
        bool               started_;
        std::mutex         lock_;
        uint32_t           suspend_interval_;
        handler *          handler_;
      };
    }
  }
}

#endif /*_csl_comm_tcp_listener_hh_included_*/
