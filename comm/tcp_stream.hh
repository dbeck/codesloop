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

#ifndef _csl_comm_tcp_stream_hh_included_
#define _csl_comm_tcp_stream_hh_included_
#include "codesloop/comm/network_stream.hh"
#include "codesloop/comm/tcp_stream_target.hh"
#include "codesloop/comm/tcp_stream_source.hh"
#ifdef __cplusplus

namespace csl
{
  namespace comm
  {
    namespace tcp
    {
      typedef csl::common::output_stream<
                      uint8_t,
                      csl::comm::tcp::stream_target,
                      csl::common::stream_buffer,
                      1024,
                      256*1024> tcp_output_stream_base;

      typedef csl::common::input_stream<
                      uint8_t,
                      csl::comm::tcp::stream_source,
                      csl::common::stream_buffer,
                      1024,
                      256*1024> tcp_input_stream_base;

      class output_stream : virtual public network_stream, virtual public tcp_output_stream_base
      {
      public:
        typedef csl::comm::tcp::stream_target<uint8_t> tcp_target_t;

        virtual ~output_stream() {}
      };

      class input_stream : virtual public network_stream, virtual public tcp_input_stream_base
      {
      public:
        typedef csl::comm::tcp::stream_source<uint8_t> tcp_source_t;

        virtual ~input_stream() {}
      };
    }
  }
}

#endif /*__cplusplus*/
#endif /*_csl_comm_tcp_stream_hh_included_*/
