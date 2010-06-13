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

#include "codesloop/common/stream.hh"

namespace csl
{
  namespace common
  {
    /* the goal of these codes is to iron out common template errors
       as early as possible by instantiating some basic combinations */

    namespace
    {
      typedef output_stream<int32_t> basic_i32_output_stream;
      typedef output_stream<uint8_t> basic_u8_output_stream;

      class i32_output_stream : public basic_i32_output_stream
      {
      public:
        i32_output_stream(stream_buffer<int32_t> & b) : basic_i32_output_stream(b) {}

        i32_output_stream( stream_nop_target<int32_t> & t,
                           stream_buffer<int32_t> & b)
          : basic_i32_output_stream(t,b) {}
      };

      void check_basic_output_streams()
      {
        {
          stream_buffer<int32_t> b;
          basic_i32_output_stream bi32os1(b);

          stream_nop_target<int32_t> t;
          basic_i32_output_stream bi32os2(t,b);

          i32_output_stream i32s1(b);
          i32_output_stream i32s2(t,b);
        }

        {
          stream_buffer<uint8_t> b;
          basic_u8_output_stream bu8os1(b);

          stream_nop_target<uint8_t> t;
          basic_u8_output_stream bu8os2(t,b);
        }
      }

      template <typename T> class source
      {
      public:
        typedef stream_flags   flags_t;
        typedef stream_base<T> stream_t;

        const flags_t & start(stream_t &)  { return flags_; }
        const flags_t & end(stream_t &)    { return flags_; }
        const flags_t & flush(stream_t &)  { return flags_; }
        const flags_t & poll(uint32_t &)   { return flags_; }

      private:
        flags_t flags_;
      };

      typedef input_stream<int32_t,source> basic_i32_input_stream;
      typedef input_stream<uint8_t,source> basic_u8_input_stream;

      class i32_input_stream : public basic_i32_input_stream
      {
      public:
        i32_input_stream( source<int32_t> & s,
                          stream_buffer<int32_t> & b)
          : basic_i32_input_stream(s,b) {}
      };

      void check_basic_input_streams()
      {
        {
          stream_buffer<int32_t> b;
          source<int32_t> s;
          basic_i32_input_stream bi32is2(s,b);
          i32_input_stream       i32is1(s,b);
        }

        {
          stream_buffer<uint8_t> b;
          source<uint8_t> s;
          basic_u8_input_stream bu8is2(s,b);
        }
      }
    }
  }
}

/* EOF */
