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

#ifndef _csl_comm_udp_listener_hh_included_
#define _csl_comm_udp_listener_hh_included_
#include "codesloop/common/excbase.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/kspin.hh"
#include "codesloop/common/ksbuf.hh"
#include "codesloop/comm/msghandler.hh"
#include "codesloop/comm/autofd.hh"
#include <thread>
#include <mutex>
#include <atomic>
#include <memory>

#ifndef CSL_COMM_UDP_LISTENER_BUFFER_COUNT
#define CSL_COMM_UDP_LISTENER_BUFFER_COUNT 512
#endif //CSL_COMM_UDP_LISTENER_BUFFER_COUNT

namespace csl
{
  namespace comm
  {
    namespace udp
    {
      class listener
      {
      public:
        CSL_CLASS( csl::comm::tcp::listener );
        CSL_DECLARE_EXCEPTION( not_started );
        CSL_DECLARE_EXCEPTION( already_started );
        CSL_DECLARE_EXCEPTION( failed_to_resolve_name );
        CSL_DECLARE_EXCEPTION( failed_to_create_socket );
        CSL_DECLARE_EXCEPTION( failed_to_set_reuseaddr );
        CSL_DECLARE_EXCEPTION( failed_to_bind );
        CSL_DECLARE_EXCEPTION( select_failed );
        CSL_DECLARE_EXCEPTION( recvfrom_failed );
        CSL_DECLARE_EXCEPTION( out_of_memory );

        listener(
            const std::string & hostname,
            const std::string & port,
            msghandler & h);

        ~listener();

        bool start();
        bool stop();

        const addr & address() const { return addr_; }

      private:
        void loop();
        listener() = delete;
        listener(const listener&) = delete;
        listener & operator=(const listener &) = delete;

        typedef common::ksbuf<CSL_COMM_UDP_LISTENER_BUFFER_COUNT> buffer_t;

        autofd                          sock_;
        std::string                     hostname_;
        std::string                     port_;
        addr                            addr_;
        std::thread                     thread_;
        std::atomic_bool                started_;
        std::atomic_bool                stop_me_;
        std::mutex                      lock_;
        uint32_t                        suspend_interval_;
        msghandler *                    handler_;
        std::auto_ptr<buffer_t>         buffer_;
      };
    }
  }
}

#endif /*_csl_comm_udp_listener_hh_included_*/
