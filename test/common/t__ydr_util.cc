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
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
#endif /* DEBUG */
//#endif

#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/ydr_util.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/test_timer.h"
#include <assert.h>

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
  }

  void baseline_i32buf()
  {
    stream_buffer<int32_t> buf;
  }

  void baseline_u8stream()
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<uint8_t> bs(buf);
  }

  void baseline_i32stream()
  {
    stream_buffer<int32_t> buf;
    buffered_stream<int32_t> bs(buf);
  }

  template <typename ST,typename TESTED>
  bool test_type(TESTED v)
  {
    stream_buffer<ST> buf;
    buffered_stream<ST> bs(buf);
    TESTED x;
    uint32_t timeout_ms = 0;
    ydr_push(bs,v);
    ydr_pop(bs,x,timeout_ms);
    return (v == x);
  }

  void test_u8_int()
  {
    assert( (test_type<uint8_t,int>(123456) == true) );
  }

  void test_i32_int()
  {
    assert( (test_type<int32_t,int>(123456) == true) );
  }

} /* end of test_ydr_util */

using namespace test_ydr_util;

int main()
{
  csl_common_print_results( "baseline_u8buf     ", csl_common_test_timer_v0(baseline_u8buf),"" );
  csl_common_print_results( "baseline_i32buf    ", csl_common_test_timer_v0(baseline_i32buf),"" );
  csl_common_print_results( "baseline_u8stream  ", csl_common_test_timer_v0(baseline_u8stream),"" );
  csl_common_print_results( "baseline_i32stream ", csl_common_test_timer_v0(baseline_i32stream),"" );

  csl_common_print_results( "u8_int             ", csl_common_test_timer_v0(test_u8_int),"" );
  //csl_common_print_results( "i32_int            ", csl_common_test_timer_v0(test_i32_int),"" );
  return 0;
}

/* EOF */
