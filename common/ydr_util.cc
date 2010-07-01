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

        u8_stream_base_t &    stream_;
        uint32_t              timeout_ms_default_;
        uint32_t &            timeout_ms_;
        stream_part_t         part_;

        ydr_convert(u8_stream_base_t & s, uint32_t & t) : stream_(s), timeout_ms_(t) {}
        ydr_convert(u8_stream_base_t & s)
          : stream_(s),
            timeout_ms_default_(0),
            timeout_ms_(timeout_ms_default_) {}


        uint8_t * reserve(size_t count)
        {
          ENTER_FUNCTION();
          stream_part_t & pf(stream_.reserve(count, part_));
          if( pf.flags().is_ok() == false ) { THROW_PUSH_EXCEPTION(pf.data(), pf.flags()); }
          RETURN_FUNCTION(pf.data());
        }

        bool confirm(size_t count)
        {
          ENTER_FUNCTION();
          stream_part_t & pf(stream_.confirm(count,part_));
          if( pf.flags().is_ok() == false ) { THROW_PUSH_EXCEPTION(false, pf.flags()); }
          RETURN_FUNCTION(true);
        }

        uint8_t * get(size_t count)
        {
          ENTER_FUNCTION();
          size_t available_items = 0;
          const stream_flags & fl(stream_.poll(count, available_items, timeout_ms_));
          if( available_items < count ) { THROW_POP_EXCEPTION(part_.data(), fl); }
          stream_part_t & pf(stream_.get(count, part_));
          if( pf.flags().is_ok() == false ) { THROW_POP_EXCEPTION(pf.data(), pf.flags()); }
          RETURN_FUNCTION(pf.data());
        }

        template <typename ITEM>
        static size_t item_count(size_t count) { return round_to_4(count*sizeof(ITEM)); }

        u8_stream_base_t & push(int32_t v)
        {
          ENTER_FUNCTION();
          uint8_t * p = reserve(sizeof(int32_t));
          int32_t le_i32 = htole32(v);
          ::memcpy(p, &le_i32, sizeof(int32_t));
          confirm(sizeof(int32_t));
          RETURN_FUNCTION(stream_);
        }

        u8_stream_base_t & push(uint32_t v)
        {
          ENTER_FUNCTION();
          uint8_t * p = reserve(sizeof(uint32_t));
          uint32_t le_u32 = htole32(v);
          ::memcpy(p, &le_u32, sizeof(uint32_t));
          confirm(sizeof(uint32_t));
          RETURN_FUNCTION(stream_);
        }

        u8_stream_base_t & push(int64_t v)
        {
          ENTER_FUNCTION();
          uint8_t * p = reserve(sizeof(int64_t));
          int64_t le_i64 = htole64(v);
          ::memcpy(p, &le_i64, sizeof(int64_t));
          confirm(sizeof(int64_t));
          RETURN_FUNCTION(stream_);
        }

        u8_stream_base_t & push(uint64_t v)
        {
          ENTER_FUNCTION();
          uint8_t * p = reserve(sizeof(uint64_t));
          uint64_t le_u64 = htole64(v);
          ::memcpy(p, &le_u64, sizeof(uint64_t));
          confirm(sizeof(uint64_t));
          RETURN_FUNCTION(stream_);
        }

        template <typename ITEM>
        u8_stream_base_t & push(ITEM & i) { return stream_; }

        u8_stream_base_t & pop(int32_t & v)
        {
          ENTER_FUNCTION();
          uint8_t * p = get(sizeof(int32_t));
          int32_t le_i32 = 0;
          ::memcpy(&le_i32,p,sizeof(int32_t));
          v = le32toh(le_i32);
          RETURN_FUNCTION(stream_);
        }

        u8_stream_base_t & pop(uint32_t & v)
        {
          ENTER_FUNCTION();
          uint8_t * p = get(sizeof(uint32_t));
          uint32_t le_u32 = 0;
          ::memcpy(&le_u32,p,sizeof(uint32_t));
          v = le32toh(le_u32);
          RETURN_FUNCTION(stream_);
        }

        u8_stream_base_t & pop(int64_t & v)
        {
          ENTER_FUNCTION();
          uint8_t * p = get(sizeof(int64_t));
          int64_t le_i64 = 0;
          ::memcpy(&le_i64,p,sizeof(int64_t));
          v = le64toh(le_i64);
          RETURN_FUNCTION(stream_);
        }

        u8_stream_base_t & pop(uint64_t & v)
        {
          ENTER_FUNCTION();
          uint8_t * p = get(sizeof(uint64_t));
          uint64_t le_u64 = 0;
          ::memcpy(&le_u64,p,sizeof(uint64_t));
          v = le64toh(le_u64);
          RETURN_FUNCTION(stream_);
        }

        template <typename ITEM>
        u8_stream_base_t & pop(ITEM & i) { return stream_; }
      };

      template <> struct ydr_convert<i32_stream_base_t>
      {
        CSL_OBJ(csl::common::anonymous,ydr_convert);
        static bool use_exc() { return true; }
        typedef i32_stream_base_t::part_t stream_part_t;

        i32_stream_base_t &   stream_;
        uint32_t              timeout_ms_default_;
        uint32_t &            timeout_ms_;
        stream_part_t         part_;

        ydr_convert(i32_stream_base_t & s, uint32_t & t)  : stream_(s), timeout_ms_(t) {}
        ydr_convert(i32_stream_base_t & s)
          : stream_(s),
            timeout_ms_default_(0),
            timeout_ms_(timeout_ms_default_) {}

        int32_t * reserve(size_t count)
        {
          ENTER_FUNCTION();
          stream_part_t & pf(stream_.reserve(count, part_));
          if( pf.flags().is_ok() == false ) { THROW_PUSH_EXCEPTION(pf.data(), pf.flags()); }
          RETURN_FUNCTION(pf.data());
        }

        bool confirm(size_t count)
        {
          ENTER_FUNCTION();
          stream_part_t & pf(stream_.confirm(count,part_));
          if( pf.flags().is_ok() == false ) { THROW_PUSH_EXCEPTION(false, pf.flags()); }
          RETURN_FUNCTION(true);
        }

        int32_t * get(size_t count)
        {
          ENTER_FUNCTION();
          size_t available_items = 0;
          const stream_flags & fl(stream_.poll(count, available_items, timeout_ms_));
          if( available_items < count ) { THROW_POP_EXCEPTION(part_.data(), fl); }
          stream_part_t & pf(stream_.get(count, part_));
          if( pf.flags().is_ok() == false ) { THROW_POP_EXCEPTION(pf.data(), pf.flags()); }
          RETURN_FUNCTION(pf.data());
        }

        template <typename ITEM>
        static size_t item_count(size_t count) {
          return (round_to_4(count*sizeof(ITEM)))/sizeof(int32_t);
        }

        i32_stream_base_t & push(int32_t v)
        {
          ENTER_FUNCTION();
          int32_t * p = reserve(sizeof(int32_t)/sizeof(int32_t));
          p[0] = htole32(v);
          confirm(sizeof(int32_t)/sizeof(int32_t));
          RETURN_FUNCTION(stream_);
        }

        i32_stream_base_t & push(uint32_t v)
        {
          ENTER_FUNCTION();
          int32_t * p = reserve(sizeof(uint32_t)/sizeof(int32_t));
          p[0] = htole32(v);
          confirm(sizeof(uint32_t)/sizeof(int32_t));
          RETURN_FUNCTION(stream_);
        }

        i32_stream_base_t & push(int64_t v)
        {
          ENTER_FUNCTION();
          int32_t * p = reserve(sizeof(int64_t)/sizeof(int32_t));
          int64_t le_i64 = htole64(v);
          ::memcpy(p,&le_i64,sizeof(le_i64));
          confirm(sizeof(int64_t)/sizeof(int32_t));
          RETURN_FUNCTION(stream_);
        }

        i32_stream_base_t & push(uint64_t v)
        {
          ENTER_FUNCTION();
          int32_t * p = reserve(sizeof(uint64_t)/sizeof(int32_t));
          uint64_t le_u64 = htole64(v);
          ::memcpy(p,&le_u64,sizeof(le_u64));
          confirm(sizeof(uint64_t)/sizeof(int32_t));
          RETURN_FUNCTION(stream_);
        }

        template <typename ITEM>
        i32_stream_base_t & push(ITEM & i) { return stream_; }

        i32_stream_base_t & pop(int32_t & v)
        {
          ENTER_FUNCTION();
          int32_t * p = get(sizeof(int32_t)/sizeof(int32_t));
          v = le32toh(p[0]);
          RETURN_FUNCTION(stream_);
        }

        i32_stream_base_t & pop(uint32_t & v)
        {
          ENTER_FUNCTION();
          int32_t * p = get(sizeof(uint32_t)/sizeof(int32_t));
          v = le32toh(p[0]);
          RETURN_FUNCTION(stream_);
        }

        i32_stream_base_t & pop(int64_t & v)
        {
          ENTER_FUNCTION();
          int32_t * p = get(sizeof(int64_t)/sizeof(int32_t));
          int64_t le_i64 = 0;
          ::memcpy(&le_i64,p,sizeof(le_i64));
          v = le64toh(le_i64);
          RETURN_FUNCTION(stream_);
        }

        i32_stream_base_t & pop(uint64_t & v)
        {
          ENTER_FUNCTION();
          int32_t * p = get(sizeof(uint64_t)/sizeof(int32_t));
          uint64_t le_u64 = 0;
          ::memcpy(&le_u64,p,sizeof(le_u64));
          v = le64toh(le_u64);
          RETURN_FUNCTION(stream_);
        }

        template <typename ITEM>
        i32_stream_base_t & pop(ITEM & i) { return stream_; }
      };

      template <typename STREAM_T, typename ITEM>
      STREAM_T & push(STREAM_T & os, ITEM i)
      {
        ydr_convert<STREAM_T> converter(os);
        return converter.push(i);
      }

      template <typename STREAM_T, typename ITEM>
      STREAM_T & pop(STREAM_T & is, ITEM & i, uint32_t & timeout_ms)
      {
        ydr_convert<STREAM_T> converter(is, timeout_ms);
        return converter.pop(i);
      }

#undef THROW_PUSH_EXCEPTION
#undef THROW_POP_EXCEPTION
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
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, int32_t val)      { return push(os, val); }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, uint32_t val)     { return push(os, val); }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, int64_t val)      { return push(os, val); }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, uint64_t val)     { return push(os, val); }
    i32_stream_base_t & ydr_push(i32_stream_base_t & os, const char * val) { return push(os, val); }
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::serializable & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::var & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::str & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const common::ustr & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const bindata_t & val);
    //i32_stream_base_t & ydr_push(i32_stream_base_t & os, const pbuf & val);

    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, int32_t & val, uint32_t & timeout_ms)  { return pop(is, val, timeout_ms); }
    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, uint32_t & val, uint32_t & timeout_ms) { return pop(is, val, timeout_ms); }
    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, int64_t & val, uint32_t & timeout_ms)  { return pop(is, val, timeout_ms); }
    i32_stream_base_t & ydr_pop(i32_stream_base_t & is, uint64_t & val, uint32_t & timeout_ms) { return pop(is, val, timeout_ms); }
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::serializable & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::var & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::str & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, common::ustr & val, uint32_t & timeout_ms);
    //i32_stream_base_t & ydr_pop(i32_stream_base_t & is, pbuf & val, uint32_t & timeout_ms);
  }
}

// EOF
