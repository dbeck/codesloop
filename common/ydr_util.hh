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
#include "codesloop/common/str.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/common/binry.hh"
#include "codesloop/common/dbl.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    namespace ydr_util
    {
      inline size_t round_to_4_bytes(size_t sz) { return ((((sz) + 3) & (~3))); }
      
      // push:
      //   - [1] calculate how many bytes to be allocated
      //   - [2] allocate (reserve)
      //   - [3] copy-in
      //   - [4] confirm
      
      // pop:
      //   - [5] calculate how many bytes to be read
      //   - [6] get ( read bytes )
      //   - [7] copy-out
      
      // -----------------------------------------------------------------------
      // [1] [5] Calculate YDR length
      inline size_t ydr_length(const int8_t & val)   { return 4; }
      inline size_t ydr_length(const uint8_t & val)  { return 4; }
      inline size_t ydr_length(const int16_t & val)  { return 4; }
      inline size_t ydr_length(const uint16_t & val) { return 4; }
      
      inline size_t ydr_length(const int32_t & val)  { return 4; }
      inline size_t ydr_length(const uint32_t & val) { return 4; }
      inline size_t ydr_length(const int64_t & val)  { return 8; }
      inline size_t ydr_length(const uint64_t & val) { return 8; }

      inline size_t ydr_length(const float & val)  { return 12; }
      inline size_t ydr_length(const double & val) { return 12; }
                  
      inline size_t ydr_length(const char * val, size_t len)
      {
        return (8+round_to_4_bytes(len));
      }
      
      inline size_t ydr_length(const str & val)   { return (8+round_to_4_bytes(val.var_size())); }
      inline size_t ydr_length(const ustr & val)  { return (8+round_to_4_bytes(val.var_size())); }
      inline size_t ydr_length(const binry & val) { return (8+round_to_4_bytes(val.var_size())); }
      inline size_t ydr_length(const dbl & val)   { return ydr_length(val.value()); }
      inline size_t ydr_length(const int64 & val) { return ydr_length(val.value()); }
      inline size_t ydr_length(const char * val)  { return (8+round_to_4_bytes(val==0 ? 0 : ::strlen(val))); }

      // -----------------------------------------------------------------------
      // [3] copy-in
      inline void ydr_copy_in_common32(void * dst, uint32_t val)
      {
        uint32_t le_u32 = htole32(val);
        ::memcpy(dst,&le_u32,sizeof(le_u32));
      }

      inline void ydr_copy_in_common64(void * dst, uint64_t val)
      {
        uint64_t le_u64 = htole64(val);
        ::memcpy(dst,&le_u64,sizeof(le_u64));
      }
                  
      inline void ydr_copy_in(void * dst, const int8_t & val)   { ydr_copy_in_common32(dst,static_cast<uint32_t>(val)); }
      inline void ydr_copy_in(void * dst, const uint8_t & val)  { ydr_copy_in_common32(dst,static_cast<uint32_t>(val)); }
      inline void ydr_copy_in(void * dst, const int16_t & val)  { ydr_copy_in_common32(dst,static_cast<uint32_t>(val)); }
      inline void ydr_copy_in(void * dst, const uint16_t & val) { ydr_copy_in_common32(dst,static_cast<uint32_t>(val)); }
      inline void ydr_copy_in(void * dst, const int32_t & val)  { ydr_copy_in_common32(dst,static_cast<uint32_t>(val)); }
      inline void ydr_copy_in(void * dst, const uint32_t & val) { ydr_copy_in_common32(dst,static_cast<uint32_t>(val)); }
      inline void ydr_copy_in(void * dst, const int64_t & val)  { ydr_copy_in_common64(dst,static_cast<uint64_t>(val)); }
      inline void ydr_copy_in(void * dst, const uint64_t & val) { ydr_copy_in_common64(dst,static_cast<uint64_t>(val)); }

      void ydr_copy_in(void * dst, const float & val);
      void ydr_copy_in(void * dst, const double & val);
      void ydr_copy_in(void * dst, const char * str, size_t len);
      void ydr_copy_in(void * dst, const str & val);
      void ydr_copy_in(void * dst, const ustr & val);
      void ydr_copy_in(void * dst, const binry & val);

      inline void ydr_copy_in(void * dst, const dbl & val)   { ydr_copy_in(dst,val.value()); }
      inline void ydr_copy_in(void * dst, const int64 & val) { ydr_copy_in(dst,val.value()); }

      // -----------------------------------------------------------------------
      // [7] copy-out
      // TODO
      inline uint32_t ydr_copy_out_common32(const void * src)
      {
        uint32_t le_u32 = 0;
        ::memcpy(&le_u32,src,sizeof(le_u32));
        uint32_t ret = le32toh(le_u32);
        return ret;
      }
      
      inline uint64_t ydr_copy_out_common64(const void * src)
      {
        uint64_t le_u64 = 0;
        ::memcpy(&le_u64,src,sizeof(le_u64));
        uint64_t ret = le64toh(le_u64);
        return ret;
      }
      
      inline void ydr_copy_out(int8_t & val,   const void * src) { val = static_cast<int8_t>(ydr_copy_out_common32(src));   }
      inline void ydr_copy_out(uint8_t & val,  const void * src) { val = static_cast<uint8_t>(ydr_copy_out_common32(src));  }
      inline void ydr_copy_out(int16_t & val,  const void * src) { val = static_cast<int16_t>(ydr_copy_out_common32(src));  }
      inline void ydr_copy_out(uint16_t & val, const void * src) { val = static_cast<uint16_t>(ydr_copy_out_common32(src)); }
      inline void ydr_copy_out(int32_t & val,  const void * src) { val = static_cast<int32_t>(ydr_copy_out_common32(src));  }
      inline void ydr_copy_out(uint32_t & val, const void * src) { val = static_cast<uint32_t>(ydr_copy_out_common32(src)); }
      inline void ydr_copy_out(int64_t & val,  const void * src) { val = static_cast<int64_t>(ydr_copy_out_common64(src));  }
      inline void ydr_copy_out(uint64_t & val, const void * src) { val = static_cast<uint64_t>(ydr_copy_out_common64(src)); }

      void ydr_copy_out(float & val,  const void * src);
      void ydr_copy_out(double & val, const void * src);

      inline void ydr_copy_out(dbl & val, const void * src)   { dbl::value_t v; ydr_copy_out(v,src); val.from_double(v); }
      inline void ydr_copy_out(int64 & val, const void * src) { int64::value_t v; ydr_copy_out(v,src); val.from_integer(v); }

    }; // end of: ns:ydr_util

#ifndef THROW_YDR_CONVERT_EXCEPTION
#define THROW_YDR_CONVERT_EXCEPTION(RETVAL,TYPE,FLAGS) \
  do {                                       \
    str flags__;                             \
    stream_flags tmp_flags__(FLAGS);         \
    tmp_flags__.to_str(flags__);             \
    throw csl::common::exc(                  \
      csl::common::exc::TYPE,                \
      L"ydr_util",                           \
      flags__.c_str(),                       \
      L""__FILE__,                           \
      __LINE__ );                            \
  } while(0)
#endif /*THROW_YDR_CONVERT_EXCEPTION*/

    template <typename T>
    stream_base & ydr_push(stream_base & os, const T & val)
    {
      // calculate the space to be allocated
      size_t sz = ydr_util::ydr_length(val);
      
      stream_base::part_t p;
      
      // alocate from the stream buffer
      stream_base::part_t & pp(os.reserve(sz,p));
      
      if( pp.flags().is_ok() )
      {
        // simple case : we have the required size
        //               so put the data into the buffer
        ydr_util::ydr_copy_in(pp.data(),val);

        // tell the stream how many bytes we used
        pp = os.confirm(sz,p);
        
        if( pp.flags().is_ok() )
        {
          // everything went smoothly
        }
        else
        {
          // cannot commit ???
          THROW_YDR_CONVERT_EXCEPTION(os,rs_stream_push,pp);
        }
      }
      else
      {
        if( pp.flags().has_flags(stream_flags::partially_allocated_) )
        {
          // not enough space for the YDR data
        }
        else if( pp.flags().has_flags(stream_flags::buffer_full_) )
        {
          // no space in the buffer
        }
        
        // give back the allocated space if any
        os.confirm(0,p);
        
        // cancel encoding
        THROW_YDR_CONVERT_EXCEPTION(os,rs_stream_push,pp);
      }
      return os;
    }

    template <typename T>
    stream_base & ydr_pop(stream_base & is, T & val, uint32_t & timeout_ms)
    {
      // calculate the required space to be allocated
      size_t sz = ydr_util::ydr_length(val);
      
      stream_base::part_t p;
      
      // check if there are enough items to be read
      size_t available_items = 0;
      const stream_flags & fl(is.poll(sz, available_items, timeout_ms));
      
      if( available_items < sz )
      {
        // not enough items within the given time 
        // TODO: rethink this for stream and string types ...
        p.flags() << stream_flags::timed_out_;
        p.flags() << fl;
        THROW_YDR_CONVERT_EXCEPTION(os,rs_stream_pop,p.flags());
      }
      else if( fl.is_ok() ==  false )
      {
        // other error in poll...
        THROW_YDR_CONVERT_EXCEPTION(os,rs_stream_pop,fl);
      }
      
      stream_base::part_t & pp(is.get(sz, p));
      
      if( pp.flags().is_ok() == false )
      {
        // bad stream part...
        THROW_YDR_CONVERT_EXCEPTION(os,rs_stream_pop,pp.flags());
      }
      
      // simple case: we have the required amount of data to be
      //              decoded
      ydr_util::ydr_copy_out(val, pp.data());
      
      return is;
    }
    
    stream_base & ydr_pop(stream_base & is, str & val, uint32_t & timeout_ms) { return is; } // TODO
    stream_base & ydr_pop(stream_base & is, ustr & val, uint32_t & timeout_ms) { return is; } // TODO
    stream_base & ydr_pop(stream_base & is, binry & val, uint32_t & timeout_ms) { return is; } // TODO

#undef THROW_YDR_CONVERT_EXCEPTION

    // stream_base
#if 0 
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
#endif //0
  }
}

#endif /*__cplusplus*/
#endif /*_csl_common_ydr_util_hh_included_*/
