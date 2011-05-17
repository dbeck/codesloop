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
      typedef output_stream<> basic_output_stream;

      class my_output_stream : public basic_output_stream
      {
      public:
        my_output_stream(stream_buffer<uint8_t> & b) : basic_output_stream(b) {}

        my_output_stream( stream_nop_target & t,stream_buffer<uint8_t> & b)
          : basic_output_stream(t,b) {}
      };

      void check_basic_output_streams()
      {
        {
          stream_buffer<uint8_t> b;
          basic_output_stream bos1(b);

          stream_nop_target t;
          basic_output_stream bos2(t,b);

          my_output_stream s1(b);
          my_output_stream s2(t,b);
        }
      }

      class my_source
      {
      public:
        typedef stream_flags   flags_t;
        typedef stream_base    stream_t;

        const flags_t & start(stream_t &)        { return flags_; }
        const flags_t & end(stream_t &)          { return flags_; }
        const flags_t & flush(stream_t &)        { return flags_; }
        const flags_t & poll(size_t,uint32_t &)  { return flags_; }

      private:
        flags_t flags_;
      };

      typedef input_stream<my_source> basic_input_stream;

      class my_input_stream : public basic_input_stream
      {
      public:
        my_input_stream( my_source & s,
                         stream_buffer<uint8_t> & b)
          : basic_input_stream(s,b) {}
      };

      void check_basic_input_streams()
      {
        {
          stream_buffer<uint8_t> b;
          my_source              s;
          basic_input_stream     bis2(s,b);
          my_input_stream        mis1(s,b);
        }
      }
    }
  }
}

/* EOF */
