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

//#if 0
#ifdef DEBUG_BUILD
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
#endif /* DEBUG */
#endif /* DEBUG_BUILD */
//#endif

#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/ydr_util.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/test_timer.h"
#include <assert.h>
#include <iostream>

using namespace csl::common;

namespace test_ydr_util
{
  /*
  ** DEBUG support --------------------------------------------------------------------
  */
  static inline const wchar_t * get_namespace()   { return L"test_ydr_util"; }
  static inline const wchar_t * get_class_name()  { return L"test_ydr_util::noclass"; }
  static inline const wchar_t * get_class_short() { return L"noclass"; }

  void baseline_u8buf()
  {
    stream_buffer<uint8_t> buf;
    buf.reset();
  }

  void baseline_u8stream()
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    buf.reset();
  }

  void baseline_u8push()
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    int32_t v = -123;
    ydr_push(bs,v);
  }

  template <typename TESTED>
  bool test_type(TESTED v)
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    TESTED x;
    uint32_t timeout_ms = 0;
    ydr_push(bs,v);
    ydr_pop(bs,x,timeout_ms);
    return (v == x);
  }

  void test_u8_int()
  {
    assert( (test_type<int>(123456) == true) );
    assert( (test_type<int>(-125) == true) );
    assert( (test_type<int>(1<<31) == true) );
  }

  void test_u8_int64()
  {
    assert( (test_type<int64_t>(123456) == true) );
    assert( (test_type<int64_t>(-125) == true) );
    assert( (test_type<int64_t>(1LL<<63) == true) );
  }

  void test_item_sizes()
  {
  }
  
  static char ydr_test_buffer[1024];

  template <typename T>
  inline bool test_ydr(const T & in)
  {
    T out;
    ydr_util::ydr_copy_in(ydr_test_buffer,in);
    ydr_util::ydr_copy_out(out,ydr_test_buffer);
    return (in == out);
  }
      
  void test_ydr_u8()
  {
    static uint8_t v = 100;
    assert( test_ydr(v) == true );
    ++v;
  }
  
  void test_ydr_i8()
  {
    static int8_t v = -100;
    assert( test_ydr(v) == true );
    ++v;
  }
  
  void test_ydr_u16()
  {
    static uint16_t v = 0xfff0;
    assert( test_ydr(v) == true );
    ++v;
  }

  void test_ydr_i16()
  {
    static int16_t v = -100;
    assert( test_ydr(v) == true );
    ++v;
  }
  
  void test_ydr_u32()
  {
    static uint32_t v = 0xffffff00UL;
    assert( test_ydr(v) == true );
    ++v;
  }
  
  void test_ydr_i32()
  {
    static int32_t v = -100;
    assert( test_ydr(v) == true );
    ++v;
  }
  
  void test_ydr_u64()
  {
    static uint64_t v = 0xffffffffffffff00ULL;
    assert( test_ydr(v) == true );
    ++v;
  }

  void test_ydr_i64()
  {
    static int64_t v = -100;
    assert( test_ydr(v) == true );
    ++v;
  }
  
  void test_ydr_float()
  {
    static float v1 = -100000.0;
    static float v2 = -2.0;
    assert( test_ydr(v1) == true );
    assert( test_ydr(v2) == true );
    v1 /= -1.1f;
    v2 *= -1.1f;
  }
  
  void test_ydr_double()
  {
    static double v1 = -100000.0;
    static double v2 = -2.0;
    assert( test_ydr(v1) == true );
    assert( test_ydr(v2) == true );
    v1 /= -1.1;
    v2 *= -1.1;
  }

} /* end of test_ydr_util */

using namespace test_ydr_util;

int main()
{
  uint64_t a = 0x0102030405060708ULL;
  unsigned char b[] = { 1,2,3,4,5,6,7,8 };
  uint64_t c = htonll(a);
  unsigned char * d = reinterpret_cast<unsigned char *>(&c);

  for( int i=0;i<8;++i )
  {
    if( d[i] != b[i] )
    {
      printf("Conversion error at:%d (%d != %d)\n",i,d[i],b[i]);
    }
  }

  assert( memcmp(b,d,8) == 0 );
  test_item_sizes();

  csl_common_print_results( "baseline_u8buf     ", csl_common_test_timer_v0(baseline_u8buf),"" );
  csl_common_print_results( "baseline_u8stream  ", csl_common_test_timer_v0(baseline_u8stream),"" );
  csl_common_print_results( "baseline_u8push    ", csl_common_test_timer_v0(baseline_u8push),"" );

  csl_common_print_results( "u8_int             ", csl_common_test_timer_v0(test_u8_int),"" );
  csl_common_print_results( "u8_int64           ", csl_common_test_timer_v0(test_u8_int64),"" );

  csl_common_print_results( "ydr_u8             ", csl_common_test_timer_v0(test_ydr_u8),"" );
  csl_common_print_results( "ydr_i8             ", csl_common_test_timer_v0(test_ydr_i8),"" );
  csl_common_print_results( "ydr_u16            ", csl_common_test_timer_v0(test_ydr_u16),"" );
  csl_common_print_results( "ydr_i16            ", csl_common_test_timer_v0(test_ydr_i16),"" );
  csl_common_print_results( "ydr_u32            ", csl_common_test_timer_v0(test_ydr_u32),"" );
  csl_common_print_results( "ydr_i32            ", csl_common_test_timer_v0(test_ydr_i32),"" );
  csl_common_print_results( "ydr_u64            ", csl_common_test_timer_v0(test_ydr_u64),"" );
  csl_common_print_results( "ydr_i64            ", csl_common_test_timer_v0(test_ydr_i64),"" );
  csl_common_print_results( "ydr_float          ", csl_common_test_timer_v0(test_ydr_float),"" );
  csl_common_print_results( "ydr_double         ", csl_common_test_timer_v0(test_ydr_double),"" );

  return 0;
}

/* EOF */
