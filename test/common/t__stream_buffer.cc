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

#if 0
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
//#define DEBUG_VERBOSE
#endif /* DEBUG */
#endif

#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/test_timer.h"
#include <assert.h>

using namespace csl::common;

namespace test_stream_buffer {

  /*
  ** DEBUG support --------------------------------------------------------------------
  */
  static inline const wchar_t * get_namespace()   { return L"test_stream_buffer"; }
  static inline const wchar_t * get_class_name()  { return L"test_stream_buffer::noclass"; }
  static inline const wchar_t * get_class_short() { return L"noclass"; }

  void baseline() { stream_buffer<char> o; }

  void basic()
  {
    stream_buffer<char> o;
    assert( o.start() == 0 );
    assert( o.len() == 0 );
    assert( o.buflen() == 0 );
  }

  void reserve()
  {
    typedef stream_buffer<char,10,20> lwp_t;
    lwp_t o;
    lwp_t::part_t rr;

    // do reserve
    lwp_t::part_t & rr2(o.reserve(2,rr));
    assert( o.n_free() == 18 );

    // check results
    assert( &rr == &rr2 );
    assert( rr.items() == 2 );
    assert( rr.flags() == stream_flags::ok_ );
    assert( rr.data() != NULL );
    assert( o.len() == 2 );
    assert( o.start() == 0 );
    assert( o.buflen() == 2 );

    // adjust 1
    lwp_t::part_t & rr3(o.confirm(1,rr));

    // check results
    assert( &rr == &rr3 );
    assert( rr.items() == 1 );
    assert( rr.flags() == stream_flags::ok_ );
    assert( rr.data() != NULL );
    assert( o.len() == 1 );
    assert( o.start() == 0 );
    assert( o.buflen() == 1 );
  }

  void reserve_max()
  {
    typedef stream_buffer<char,10,20> lwp_t;
    lwp_t o;
    lwp_t::part_t rr;

    // reserve 1: 8
    o.reserve(8,rr);
    assert( rr.items() == 8 );
    assert( o.len() == 8 );
    assert( o.start() == 0 );
    assert( o.buflen() == 8 );
    assert( o.n_free() == 12 );

    // reserve 2: +8 =  16
    o.reserve(8,rr);
    assert( rr.items() == 8 );
    assert( o.len() == 16 );
    assert( o.start() == 0 );
    assert( o.buflen() == 16 );
    assert( o.n_free() == 4 );

    // reserve 3: +8 = 24 > 20 => 20
    o.reserve(8,rr);
    assert( rr.items() == 4 );
    assert( o.len() == 20 );
    assert( o.start() == 0 );
    assert( o.buflen() == 20 );
    assert( o.n_free() == 0 );
  }

  void reserve_badinput()
  {
    typedef stream_buffer<char,10,20> lwp_t;
    lwp_t o;
    lwp_t::part_t rr;
    o.reserve(0,rr);
    assert( o.n_free() == 20 );

    assert( rr.items() == 0 );
    assert( rr.data() == NULL );
    assert( o.len() == 0 );
    assert( o.start() == 0 );
    assert( o.buflen() == 0 );

    rr.data( reinterpret_cast<lwp_t::item_t *>(33ULL) );     // bad ptr
    rr.items( 999999ULL );                                   // bad size
    rr.flags() << ( stream_flags::timed_out_ | stream_flags::application_error_ |
                    stream_flags::os_error_  | stream_flags::try_again_ );

    o.reserve(12,rr);
    assert( o.n_free() == 8 );
    o.reserve(0,rr);
    assert( o.n_free() == 8 );

    assert( rr.items() == 0 );
    assert( rr.data() == NULL );
    assert( o.len() == 12 );
    assert( o.start() == 0 );
    assert( o.buflen() == 12 );
  }

  void adjust()
  {
    typedef stream_buffer<char,10,20> lwp_t;
    lwp_t o;
    lwp_t::part_t rr;

    o.reserve(6,rr);
    assert( o.n_free() == 14 );
    o.confirm(4,rr);
    assert( o.n_free() == 16 );

    assert( rr.items() == 4 );
    assert( rr.data() != NULL );
    assert( o.len() == 4 );
    assert( o.start() == 0 );
    assert( o.buflen() == 4 );

    rr.reset();
    o.reserve(4,rr);
    assert( o.n_free() == 12 );
    assert( rr.items() == 4 );
    assert( rr.data() != NULL );
    assert( o.len() == 8 );
    assert( o.buflen() == 8 );

    // 1 item succeed out of 4 items reserved
    // this means, 3 items should be cut off
    o.confirm(1,rr);
    assert( o.n_free() == 15 );
    assert( rr.items() == 1 );
    assert( rr.data() != NULL );
    assert( o.len() == 5 );
    assert( o.buflen() == 5 );
  }

  void adjust_max()
  {
    typedef stream_buffer<char,10,20> o_t;
    o_t o;
    o_t::part_t rr;

    o.reserve(o_t::max_size_,rr);
    assert( rr.items() == o_t::max_size_ );
    assert( rr.data() != NULL );
    assert( o.len() == o_t::max_size_ );
    assert( o.start() == 0 );
    assert( o.buflen() == o_t::max_size_ );
    assert( o.n_free() == 0 );

    o.confirm(o_t::max_size_,rr);
    assert( rr.items() == o_t::max_size_ );
    assert( rr.data() != NULL );
    assert( o.len() == o_t::max_size_ );
    assert( o.start() == 0 );
    assert( o.buflen() == o_t::max_size_ );

    o.confirm(0,rr);
    assert( rr.items() == 0 );
    assert( rr.data() == NULL );
    assert( o.len() == 0 );
    assert( o.start() == 0 );
    assert( o.buflen() == 0 );
  }

  void adjust_badinput()
  {
    typedef stream_buffer<double,10,20> o_t;
    o_t o;
    o_t::part_t rr,rr2;

    o.reserve(13,rr2);
    rr = rr2;
    rr.data( reinterpret_cast<o_t::item_t *>(33ULL) ); // bad ptr

    // this should set error flags, because we are confirming something
    // that was not previously reserved
    o.confirm(1,rr);
    assert( rr.flags().has_flags(stream_flags::parameter_error_) );

    rr.reset();
    rr.data( rr2.data() );                         // fix pointer
    rr.items( 999999ULL );                         // bad size

    // this should set error flags
    o.confirm(1,rr);
    assert( rr.flags().has_flags(stream_flags::parameter_error_) );

    rr.items( rr2.items() );                       // fix size
    rr.flags() << ( stream_flags::os_error_ );     // failed

    // this should set error flags and keep original errors too
    o.confirm(1,rr);
    assert( rr.flags().has_flags(stream_flags::parameter_error_|stream_flags::os_error_) );
  }

  void get()
  {
    typedef stream_buffer<long long,10,20> o_t;
    o_t o;
    o_t::part_t rr,rr2,rr3;

    assert( o.n_free() == o_t::max_size_ );
    o.reserve(20,rr);
    assert( o.n_free() == 0 );

    o_t::part_t & rf2(o.get(2,rr2));
    assert( &rf2 == &rr2 );

    assert( o.len() == 18 );
    assert( o.buflen() == 20 );
    assert( o.start() == 2 );
    assert( o.n_free() == 0 );
    assert( rf2.items() == 2 );
    assert( rf2.data() == rr.data() );
    assert( rf2.flags() == stream_flags::ok_ );

    o_t::part_t & rf3(o.get(3,rr3));
    assert( o.len() == 15 );
    assert( o.buflen() == 20 );
    assert( o.start() == 5 );
    assert( o.n_free() == 0 );
    assert( rf3.items() == 3 );
    assert( rf3.data() == (rr.data()+2) );
    assert( rf3.flags() == stream_flags::ok_ );
  }

  void get_max()
  {
    typedef stream_buffer<short,10,20> o_t;
    o_t o;
    o_t::part_t rr;

    o.get(2,rr);
    // get should not change anything on stream_buffer
    assert( o.len() == 0 );
    assert( o.n_free() == o_t::max_size_ );
    assert( o.buflen() == 0 );
    assert( o.start() == 0 );
    // rr should not be changed either
    assert( rr.items() == 0 );
    assert( rr.data() == NULL );
    assert( rr.flags() == stream_flags::empty_buffer_ );

    o.reserve(9,rr);
    // check the results of reserve
    assert( o.len() == 9 );
    assert( o.n_free() == (o_t::max_size_-9) );
    assert( o.buflen() == 9 );
    assert( o.start() == 0 );
    //
    assert( rr.items() == 9 );
    assert( rr.data() != NULL );
    assert( rr.flags() == stream_flags::ok_ );

    o.get(2*o_t::max_size_,rr);
    // get should reset stream_buffer as it should have received all the data
    assert( o.len() == 0 );
    assert( o.n_free() == o_t::max_size_ );
    assert( o.buflen() == 9 );
    assert( o.start() == 0 );

    // rr have the data
    assert( rr.items() == 9 );
    assert( rr.data() != NULL );
    assert( rr.flags() == stream_flags::ok_ );

    // get/rewind
    o.reserve(14,rr);
    o.get(4,rr);
    assert( o.start() == 4 );
    o.get(21,rr);

    assert( o.len() == 0 );
    assert( o.n_free() == o_t::max_size_ );
    assert( o.buflen() == 14 );
    assert( o.start() == 0 );

    assert( rr.items() == 10 );
    assert( rr.data() != NULL );
    assert( rr.flags() == stream_flags::ok_ );
  }

  void get_badinput()
  {
    typedef stream_buffer<float,10,20> o_t;
    o_t o;
    o_t::part_t rr;
    o.reserve(9,rr);
    o.get(0,rr);

    assert( o.len() == 9 );
    assert( o.n_free() == o_t::max_size_-9 );
    assert( o.buflen() == 9 );
    assert( o.start() == 0 );

    assert( rr.items() == 0 );
    assert( rr.data() == NULL );
    assert( rr.flags() == stream_flags::parameter_error_ );
  }

} /* end of test_stream_buffer */

using namespace test_stream_buffer;

int main()
{
  csl_common_print_results( "baseline          ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "basic             ", csl_common_test_timer_v0(basic),"" );
  csl_common_print_results( "reserve           ", csl_common_test_timer_v0(reserve),"" );
  csl_common_print_results( "reserve_max       ", csl_common_test_timer_v0(reserve_max),"" );
  csl_common_print_results( "reserve_badinput  ", csl_common_test_timer_v0(reserve_badinput),"" );
  csl_common_print_results( "adjust            ", csl_common_test_timer_v0(adjust),"" );
  csl_common_print_results( "adjust_max        ", csl_common_test_timer_v0(adjust_max),"" );
  csl_common_print_results( "adjust_badinput   ", csl_common_test_timer_v0(adjust_badinput),"" );
  csl_common_print_results( "get               ", csl_common_test_timer_v0(get),"" );
  csl_common_print_results( "get_max           ", csl_common_test_timer_v0(get_max),"" );
  csl_common_print_results( "get_badinput      ", csl_common_test_timer_v0(get_badinput),"" );
  return 0;
}

/* EOF */

