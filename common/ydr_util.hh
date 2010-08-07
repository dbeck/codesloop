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

#ifndef _csl_common_ydr_util_hh_included_
#define _csl_common_ydr_util_hh_included_
#include "codesloop/common/stream.hh"
#include "codesloop/common/common.h"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    namespace ydr_util
    {
      inline size_t round_to_4_bytes(size_t sz) { return ((((sz) + 3) & (~3))); }

      // common case for some builtin types
      template <typename T> struct item_size
      {
        static inline const size_t calc_bytes(size_t item_count)
        {
          return round_to_4_bytes(item_count * sizeof(T));
        }
      };

      template <> struct item_size<const char *>
      {
        static inline const size_t 
	  calc_bytes(size_t item_count) { return (sizeof(uint64_t))+round_to_4_bytes(item_count); }
      };

      template <> struct item_size<int32_t>
      {
        static inline const size_t 
	  calc_bytes(size_t item_count) { return (4*item_count); }
      };

      template <> struct item_size<uint32_t>
      {
        static inline const size_t 
	  calc_bytes(size_t item_count) { return (4*item_count); }
      };

      template <> struct item_size<int64_t>
      {
        static inline const size_t 
	  calc_bytes(size_t item_count) { return (8*item_count); }
      };

      template <> struct item_size<uint64_t>
      {
        static inline const size_t 
	  calc_bytes(size_t item_count) { return (8*item_count); }
      };

      

    };

    // stream_base
    stream_base & ydr_push(stream_base & os, int32_t val);
    stream_base & ydr_push(stream_base & os, uint32_t val);
    stream_base & ydr_push(stream_base & os, int64_t val);
    stream_base & ydr_push(stream_base & os, uint64_t val);
    stream_base & ydr_push(stream_base & os, const char * val);
    //stream_base & ydr_push(stream_base & os, const common::serializable & val);
    //stream_base & ydr_push(stream_base & os, const common::var & val);
    //stream_base & ydr_push(stream_base & os, const common::str & val);
    //stream_base & ydr_push(stream_base & os, const common::ustr & val);
    //stream_base & ydr_push(stream_base & os, const bindata_t & val);
    //stream_base & ydr_push(stream_base & os, const pbuf & val);

    stream_base & ydr_pop(stream_base & is, int32_t & val, uint32_t & timeout_ms);
    stream_base & ydr_pop(stream_base & is, uint32_t & val, uint32_t & timeout_ms);
    stream_base & ydr_pop(stream_base & is, int64_t & val, uint32_t & timeout_ms);
    stream_base & ydr_pop(stream_base & is, uint64_t & val, uint32_t & timeout_ms);
    //stream_base & ydr_pop(stream_base & is, common::serializable & val, uint32_t & timeout_ms);
    //stream_base & ydr_pop(stream_base & is, common::var & val, uint32_t & timeout_ms);
    //stream_base & ydr_pop(stream_base & is, common::str & val, uint32_t & timeout_ms);
    //stream_base & ydr_pop(stream_base & is, common::ustr & val, uint32_t & timeout_ms);
    //stream_base & ydr_pop(stream_base & is, pbuf & val, uint32_t & timeout_ms);
  }
}

#endif /*__cplusplus*/
#endif /*_csl_common_ydr_util_hh_included_*/
