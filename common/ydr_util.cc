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

#include "codesloop/common/logger.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/ydr_util.hh"
#include "codesloop/common/obj.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/common.h"

#ifdef LINUX
#include <endian.h>
#endif

#ifdef FREEBSD
#include <sys/endian.h>
#endif

#include <math.h>

namespace csl
{
  namespace common
  {
    namespace ydr_util
    {
      namespace
      {
        static inline void double_conv(double in, int & out1, int64_t & out2)
        {
          if( in == -INFINITY )
          {
            out1 = 0xffffffff;
            out2 = 0xffffffffffffffffULL;
            return;
          }
          else if( in == INFINITY )
          {
            out1 = 0xffffffff;
            out2 = 0xefffffffffffffffULL;
            return;
          }
          else if( in == NAN )
          {
            out1 = 0xffffffff;
            out2 = 0xdfffffffffffffffULL;
            return;
          }
          double r = frexp(in, &out1);
          r *= (1LL<<62);
          out2 = static_cast<int64_t>(r);
        }
        
        static inline double double_vnoc(int in1, int64_t in2)
        {
          if( in1 == static_cast<int>(0xffffffff) )
          {
            if( in2 == static_cast<int64_t>(0xffffffffffffffffULL) )
            {
              return -INFINITY;
            }
            else if( in2 == static_cast<int64_t>(0xefffffffffffffffULL) )
            {
              return INFINITY;
            }
            else if( in2 == static_cast<int64_t>(0xdfffffffffffffffULL) )
            {
              return NAN;
            }
          }
          double r = static_cast<double>(in2);
          r /= (1LL<<62);
          return (ldexp(r,in1));
        }
      }  // end of: ns:ydr_util:anonymous
      
      void ydr_copy_in(void * dst, const float & val)
      {
        unsigned char * p = static_cast<unsigned char *>(dst);
        double v          = static_cast<double>(val);
        int res1          = 0;
        int64_t res2      = 0;
        double_conv(v,res1,res2);
        ydr_copy_in(p,res1);
        ydr_copy_in(p+ydr_length(res1),res2);
      }

      void ydr_copy_in(void * dst, const double & val)
      {
        unsigned char * p = static_cast<unsigned char *>(dst);
        int res1          = 0;
        int64_t res2      = 0;
        double_conv(val,res1,res2);
        ydr_copy_in(p,res1);
        ydr_copy_in(p+ydr_length(res1),res2);
      }

      void ydr_copy_in_ptr(void * dst, const void * buf, uint64_t sz)
      {
        unsigned char * p = static_cast<unsigned char *>(dst);
        ydr_copy_in(p,sz);
        p += ydr_length(sz);
        ::memcpy(p,buf,static_cast<size_t>(sz));
      }

      void ydr_copy_in(void * dst, const char * str, size_t len) { ydr_copy_in_ptr(dst,str,len); }
      void ydr_copy_in(void * dst, const str & val)              { ydr_copy_in_ptr(dst,val.ucharp_data(),val.var_size()); }
      void ydr_copy_in(void * dst, const ustr & val)             { ydr_copy_in_ptr(dst,val.ucharp_data(),val.var_size()); }
      void ydr_copy_in(void * dst, const binry & val)            { ydr_copy_in_ptr(dst,val.ucharp_data(),val.var_size()); }
      
      void ydr_copy_out(float & val,  const void * src) 
      {
        const unsigned char * p = static_cast<const unsigned char *>(src);
        int src1                = 0;
        int64_t src2            = 0;
        ydr_copy_out(src1,p);
        ydr_copy_out(src2,p+4);
        val = static_cast<float>(double_vnoc(src1,src2));
      }

      void ydr_copy_out(double & val, const void * src)
      {
        const unsigned char * p = static_cast<const unsigned char *>(src);
        int src1                = 0;
        int64_t src2            = 0;
        ydr_copy_out(src1,p);
        ydr_copy_out(src2,p+4);
        val = double_vnoc(src1,src2);
      }
    }  // end of: ns:ydr_util

    namespace
    {
      template <typename T>
      struct ydr_pop_var_type
      {
        stream_base & ydr_pop(stream_base & is, T & val, uint32_t & timeout_ms)
        {
          // get length from the stream
          uint64_t len = 0;
          csl::common::ydr_pop<uint64_t>(is, len, timeout_ms);
      
          // this is OK. zero length data...
          if( len == 0 ) return is;
          size_t aligned_len = ydr_util::round_to_4_bytes(static_cast<size_t>(len));

          // check if there are enough items to be read
          stream_base::part_t p;
          size_t available_items = 0;
          const stream_flags & fl(is.poll(aligned_len, available_items, timeout_ms));
     
          if( available_items < aligned_len )
          {
            // not enough items within the given time 
            // TODO: rethink this for the case of partial results
            p.flags() << stream_flags::timed_out_;
            p.flags() << fl;
            THROW_YDR_CONVERT_EXCEPTION(is, rs_stream_pop, p.flags());
          }
          else if( fl.is_ok() ==  false )
          {
            // other error in poll...
            THROW_YDR_CONVERT_EXCEPTION(is, rs_stream_pop, fl);
          }
      
          stream_base::part_t & pp(is.get(aligned_len, p));
      
          if( pp.flags().is_ok() == false )
          {
            // bad stream part...
            THROW_YDR_CONVERT_EXCEPTION(is, rs_stream_pop, pp.flags());
          }
                              
          // simple case: we have the required amount of data to be
          //              decoded
          val.from_binary(pp.data(), static_cast<size_t>(len));
          return is;
        }
      };
    }

    stream_base & ydr_pop(stream_base & is, str & val, uint32_t & timeout_ms)
    {
      ydr_pop_var_type<str> converter;
      return converter.ydr_pop(is,val,timeout_ms);
    }
    
    stream_base & ydr_pop(stream_base & is, ustr & val, uint32_t & timeout_ms)
    {
      ydr_pop_var_type<ustr> converter;
      return converter.ydr_pop(is,val,timeout_ms);
    }
    
    stream_base & ydr_pop(stream_base & is, binry & val, uint32_t & timeout_ms)
    {
      ydr_pop_var_type<binry> converter;
      return converter.ydr_pop(is,val,timeout_ms);
    }
  }
}

// EOF
