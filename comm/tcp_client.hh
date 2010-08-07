/*
Copyright (c) 2008,2009,2010, CodeSLoop Team

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

#ifndef _csl_comm_tcp_client_hh_included_
#define _csl_comm_tcp_client_hh_included_

#include "codesloop/comm/tcp_stream.hh"
#include "codesloop/comm/endpoint.hh"
#include "codesloop/comm/client.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#ifdef __cplusplus
#include <memory>

namespace csl
{
  namespace comm
  {
    namespace tcp
    {
      class client : public csl::comm::client
      {
        public:
          client();
          virtual ~client();

          bool init( channel_factory & chn_ftry,
                     endpoint & me,
                     endpoint & peer );

          bool start();
          bool stop();

          channel & get_channel();

          const endpoint & own_addr()  const { return own_addr_;  }
          const endpoint & peer_addr() const { return peer_addr_; }

        private:
          typedef csl::comm::tcp::output_stream           out_stream_t;
          typedef csl::comm::tcp::input_stream            in_stream_t;
          typedef out_stream_t::buffer_t                  out_buf_t;
          typedef in_stream_t::buffer_t                   in_buf_t;
          typedef csl::comm::tcp::stream_target           target_t;
          typedef csl::comm::tcp::stream_target           source_t;

          endpoint                own_addr_;
          endpoint                peer_addr_;
          channel_factory *       chn_ftry_;

          /* this is the abstraction seen by the users */
          std::auto_ptr<channel> channel_;

          /* these are the internal details needed */
          std::auto_ptr<target_t>      target_;
          std::auto_ptr<target_t>      source_;

          std::auto_ptr<out_stream_t>  out_buf_;
          std::auto_ptr<out_stream_t>  in_buf_;

          std::auto_ptr<out_stream_t>  out_;
          std::auto_ptr<in_stream_t>   in_;

          /* no-copy */
          client(const client & other) { }
          client & operator=(const client & other) { return *this; }

          /* for trace and debug */
          CSL_OBJ(csl::comm::tcp,client);
          USE_EXC();
      };
    }
  }
}

#endif /*__cplusplus*/
#endif /* _csl_comm_tcp_client_hh_included_ */
