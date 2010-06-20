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
#include <arpa/inet.h>

namespace csl
{
  namespace common
  {
    namespace
    {
#ifndef THROW_PUSH_EXCEPTION
#define THROW_PUSH_EXCEPTION(RETVAL, FLAGS)  \
  do {                                       \
    str flags__;                             \
    stream_flags tmp_flags__(FLAGS);         \
    tmp_flags__.to_str(flags__);             \
    THRS(csl::common::exc::rs_stream_push,   \
         flags__.c_str(),                    \
         RETVAL);                            \
  } while(0)
#endif

#ifndef THROW_POP_EXCEPTION
#define THROW_POP_EXCEPTION(RETVAL, FLAGS)   \
  do {                                       \
    stream_flags tmp_flags__(FLAGS);         \
    str flags__;                             \
    tmp_flags__.to_str(flags__);             \
    THRS(csl::common::exc::rs_stream_pop,    \
         flags__.c_str(),                    \
         RETVAL);                            \
  } while(0)
#endif

      static size_t round_to_4(size_t sz) { return ((((sz) + 3) & (~3))); }

      template <typename STREAM_T> struct ydr_convert {};

      template <> struct ydr_convert<u8_stream_base_t>
      {
        CSL_OBJ(csl::common::anonymous,ydr_convert);
        static bool use_exc() { return true; }

        typedef u8_stream_base_t::part_t stream_part_t;

        template <typename ITEM>
        static size_t item_count(size_t count)
        {
          return round_to_4(count*sizeof(ITEM));
        }

        static u8_stream_base_t & push(u8_stream_base_t & os, int32_t v)
        {
          stream_part_t part;
          stream_part_t & pf(os.reserve(4, part));
          if( pf.flags().is_ok() == false ) { THROW_PUSH_EXCEPTION(os, pf.flags());}
          int32_t network_order_i32 = htonl(v);
          ::memcpy(pf.data(), &network_order_i32, 4);
          pf = os.confirm(4,part);
          if( pf.flags().is_ok() == false ) { THROW_PUSH_EXCEPTION(os, pf.flags()); }
          return os;
        }

        template <typename ITEM>
        static u8_stream_base_t & push(u8_stream_base_t & os, ITEM & i)
        {
          size_t n_items = item_count<ITEM>(1);
          stream_part_t part;
          stream_part_t & pf(os.reserve(n_items, part));
          // XXX TODO
          return os;
        }

        static u8_stream_base_t & pop(u8_stream_base_t & is, int32_t & v, uint32_t & timeout_ms)
        {
          stream_part_t part;
          size_t available_items = 0;
          const stream_flags & fl(is.poll(4, available_items, timeout_ms));
          if( available_items < 4 ) { THROW_POP_EXCEPTION(is, fl); }
          stream_part_t & pf(is.get(4, part));
          if( pf.flags().is_ok() == false ) { THROW_POP_EXCEPTION(is, pf.flags()); }
          int32_t network_order_i32 = 0;
          ::memcpy(&network_order_i32,pf.data(),4);
          v = ntohl(network_order_i32);
          return is;
        }

        template <typename ITEM>
        static u8_stream_base_t & pop(u8_stream_base_t & is, ITEM & i, uint32_t & timeout_ms)
        {
          size_t n_items = item_count<ITEM>(1);
          stream_part_t part;
          stream_part_t & pf(is.reserve(n_items, part));
          // XXX TODO
          return is;
        }

      };

      template <> struct ydr_convert<i32_stream_base_t>
      {
        CSL_OBJ(csl::common::anonymous,ydr_convert);
        static bool use_exc() { return true; }

        template <typename ITEM>
        static size_t item_count(size_t count)
        {
          return (round_to_4(count*sizeof(ITEM)))/sizeof(int32_t);
        }
      };

      template <typename STREAM_T, typename ITEM>
      STREAM_T & push(STREAM_T & os, ITEM i)
      {
        return ydr_convert<STREAM_T>::push(os, i);
      }

      template <typename STREAM_T, typename ITEM>
      STREAM_T & pop(STREAM_T & os, ITEM & i, uint32_t & timeout_ms)
      {
        return ydr_convert<STREAM_T>::pop(os, i, timeout_ms);
      }

#undef THROW_PUSH_EXCEPTION
    }

    // u8_stream_base_t
    u8_stream_base_t & ydr_push(u8_stream_base_t & os, int32_t val)      { return push(os, val); }
    u8_stream_base_t & ydr_push(u8_stream_base_t & os, uint32_t val)     { return push(os, val); }
    u8_stream_base_t & ydr_push(u8_stream_base_t & os, int64_t val)      { return push(os, val); }
    u8_stream_base_t & ydr_push(u8_stream_base_t & os, uint64_t val)     { return push(os, val); }
    u8_stream_base_t & ydr_push(u8_stream_base_t & os, const char * val) { return push(os, val); }

    //u8_stream_base_t & ydr_push(u8_stream_base_t & os, const common::serializable & val);
    //u8_stream_base_t & ydr_push(u8_stream_base_t & os, const common::var & val);
    //u8_stream_base_t & ydr_push(u8_stream_base_t & os, const common::str & val);
    //u8_stream_base_t & ydr_push(u8_stream_base_t & os, const common::ustr & val);
    //u8_stream_base_t & ydr_push(u8_stream_base_t & os, const bindata_t & val);
    //u8_stream_base_t & ydr_push(u8_stream_base_t & os, const pbuf & val);

    u8_stream_base_t & ydr_pop(u8_stream_base_t & is, int32_t & val, uint32_t & timeout_ms)   { return pop(is, val, timeout_ms); }
    u8_stream_base_t & ydr_pop(u8_stream_base_t & is, uint32_t & val, uint32_t & timeout_ms)  { return pop(is, val, timeout_ms); }
    u8_stream_base_t & ydr_pop(u8_stream_base_t & is, int64_t & val, uint32_t & timeout_ms)   { return pop(is, val, timeout_ms); }
    u8_stream_base_t & ydr_pop(u8_stream_base_t & is, uint64_t & val, uint32_t & timeout_ms)  { return pop(is, val, timeout_ms); }
    //u8_stream_base_t & ydr_pop(u8_stream_base_t & is, common::serializable & val, uint32_t & timeout_ms);
    //u8_stream_base_t & ydr_pop(u8_stream_base_t & is, common::var & val, uint32_t & timeout_ms);
    //u8_stream_base_t & ydr_pop(u8_stream_base_t & is, common::str & val, uint32_t & timeout_ms);
    //u8_stream_base_t & ydr_pop(u8_stream_base_t & is, common::ustr & val, uint32_t & timeout_ms);
    //u8_stream_base_t & ydr_pop(u8_stream_base_t & is, pbuf & val, uint32_t & timeout_ms);

    // i32_stream_base_t
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, int32_t val)      { return os; }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, uint32_t val)     { return os; }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, int64_t val)      { return os; }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, uint64_t val)     { return os; }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, const char * val) { return os; }
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::serializable & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::var & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::str & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::ustr & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const bindata_t & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const pbuf & val);

    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, int32_t & val, uint32_t & timeout_ms)  { return is; }
    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, uint32_t & val, uint32_t & timeout_ms) { return is; }
    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, int64_t & val, uint32_t & timeout_ms)  { return is; }
    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, uint64_t & val, uint32_t & timeout_ms) { return is; }
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::serializable & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::var & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::str & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::ustr & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, pbuf & val, uint32_t & timeout_ms);
  }
}

// EOF
