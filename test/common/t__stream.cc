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

#include "codesloop/common/stream_nop_target.hh"
#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include <assert.h>

using namespace csl::common;

namespace test_stream {

  class counting_target
  {
    public:
      uint32_t start_count_;
      uint32_t end_count_;
      uint32_t flush_count_;
      uint32_t data_count_;
      size_t   data_amount_;

      const stream_flags & start(stream_base &)
      {
        ++start_count_;
        return flags_;
      }

      const stream_flags & end(stream_base &)
      {
        ++end_count_;
        return flags_;
      }

      const stream_flags & flush(stream_base &)
      {
        ++flush_count_;
        return flags_;
      }

      const stream_flags & data(stream_base & s, size_t sz)
      {
        ++data_count_;
        data_amount_ += sz;
        return flags_;
      }
      
      counting_target() : start_count_(0),
                          end_count_(0),
                          flush_count_(0),
                          data_count_(0),
                          data_amount_(0) {}

      stream_flags flags_;
  };

  void test_baseline_nop_target()
  {
    stream_nop_target t;
  }

  void test_baseline_buffer()
  {
    stream_buffer<uint8_t> b;
  }

  void test_baseline_stream()
  {
    stream_buffer<uint8_t> b;
    output_stream<> s(b);
  }
  
  void test_flags()
  {
    stream_buffer<uint8_t,1,2> b;
    typedef output_stream<stream_nop_target,stream_buffer,1,2> u8stream_t;
    
    u8stream_t s(b);
    // initial flags are ok_
    assert( s.flags() == stream_flags::ok_ );
        
    // get from empty buffer
    u8stream_t::part_t p;
    u8stream_t::part_t & sp(s.get(10,p));
    
    assert( p.flags() & stream_flags::empty_buffer_ );
    assert( sp.flags() & stream_flags::empty_buffer_ );
  }
  
  void test_reserve_ok()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    u8stream_t::part_t sp;
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( s.total_confirmed_items() == 1 );
  }
  
  void test_confirm_ok()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    u8stream_t::part_t sp;
    s.reserve(2,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( t.data_count_ == 2 );
    assert( t.data_amount_ == 2 );
    assert( s.total_confirmed_items() == 2 );
  }

  void test_get_ok()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    u8stream_t::part_t sp;
    s.reserve(2,sp);
    assert( s.total_confirmed_items() == 0 );
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( s.total_confirmed_items() == 1 );
    assert( sp.flags() == stream_flags::ok_ );
    s.get(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( s.total_confirmed_items() == 1 );
  }
  
  void test_start_ok()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    assert( s.start() == stream_flags::ok_ );
    assert( t.start_count_ == 1 );
    assert( s.total_confirmed_items() == 0 );
  }

  void test_end_ok()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    assert( s.end() == stream_flags::ok_ );
    assert( t.end_count_ == 1 );
    assert( s.total_confirmed_items() == 0 );
  }
  
  void test_flush_ok()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    assert( s.flush() == stream_flags::ok_ );
    assert( t.flush_count_ == 1 );
    assert( s.total_confirmed_items() == 0 );
  }

  void test_reserve_failed()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    u8stream_t::part_t sp;
    
    // need non-zero items
    s.reserve(0,sp);
    assert( sp.flags() & stream_flags::parameter_error_ );
    
    // fill the whole buffer
    s.reserve(2,sp);
    assert( sp.flags() == stream_flags::ok_ );
    
    // confirm that too
    s.confirm(2,sp);
    assert( sp.flags() == stream_flags::ok_ );

    // try to reserve
    s.reserve(2,sp);
    assert( sp.flags() & stream_flags::buffer_full_ );
    assert( s.flags() & stream_flags::buffer_full_ );
    
    // get all items
    s.get(2,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( s.flags() == stream_flags::ok_ );
    
    // try to allocate more than available
    s.reserve(3,sp);
    assert( sp.flags() & stream_flags::partially_allocated_ );
    assert( s.flags() & stream_flags::partially_allocated_ );
    assert( sp.items() == 2 );
    s.get(4,sp);
    assert( sp.items() == 2 );
  }

  void test_confirm_failed()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    u8stream_t::part_t sp,sp2;
    
    // n_succeed > avail : parameter_error_
    assert( s.confirm(1,sp).flags() == stream_flags::parameter_error_ );
    
    // sp.data() invalid : parameter_error_
    sp.reset();
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    sp2 = sp;
    sp2.data(sp.data()+1);
    assert( s.confirm(1,sp2).flags() == stream_flags::parameter_error_ );
    assert( sp.data()+1 == sp2.data() );
    assert( sp.items() == sp2.items() );
    assert( sp.items() == 1 );
    
    // sp.items() invalid : parameter_error_
    sp.flags().reset();
    sp.items(0);
    assert( s.confirm(1,sp).flags() == stream_flags::parameter_error_ );
    sp.flags().reset();
    sp.items(2);
    assert( s.confirm(1,sp).flags() == stream_flags::parameter_error_ );
    sp.flags().reset();
    sp.items(1);
    assert( s.confirm(1,sp).flags() == stream_flags::ok_ );
  }
  
  void test_get_failed()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    u8stream_t::part_t sp;
    
    // sz == 0 : parameter_error_
    s.reserve(1,sp);
    sp.reset();
    assert( s.get(0,sp).flags() == stream_flags::parameter_error_ );
    
    // b.len() == 0 : empty_buffer_
    s.get(100,sp);
    assert( b.len() == 0 );
    sp.reset();
    assert( s.get(1,sp).flags() == stream_flags::empty_buffer_ );
  }
  
  void test_start_failed()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    t.flags_ << stream_flags::security_error_;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    assert( s.start() == stream_flags::security_error_ );
    assert( t.start_count_ == 1 );
  }
  
  void test_end_failed()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    t.flags_ << stream_flags::security_error_;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    assert( s.end() == stream_flags::security_error_ );
    assert( t.end_count_ == 1 );
  }
  
  void test_flush_failed()
  {
    stream_buffer<uint8_t,1,2> b;
    counting_target t;
    t.flags_ << stream_flags::security_error_;
    typedef output_stream<counting_target,stream_buffer,1,2> u8stream_t;
    u8stream_t s(t,b);
    assert( s.flush() == stream_flags::security_error_ );
    assert( t.flush_count_ == 1 );
  }
}

using namespace test_stream;

int main()
{
#ifdef DEBUG

  test_reserve_ok();
  test_confirm_ok();
  test_get_ok();
  test_start_ok();
  test_end_ok();
  test_flush_ok();

  test_reserve_failed();
  test_confirm_failed();
  test_get_failed();
  test_start_failed();
  test_end_failed();
  test_flush_failed();
      
  test_flags();
  test_baseline_stream();
  
#else

  csl_common_print_results( "baseline nop target ", csl_common_test_timer_v0(test_baseline_nop_target),"" );
  csl_common_print_results( "baseline buffer     ", csl_common_test_timer_v0(test_baseline_buffer),"" );
  csl_common_print_results( "baseline stream     ", csl_common_test_timer_v0(test_baseline_stream),"" );
  csl_common_print_results( "flags               ", csl_common_test_timer_v0(test_flags),"" );
  
  csl_common_print_results( "reserve ok          ", csl_common_test_timer_v0(test_reserve_ok),"" );
  csl_common_print_results( "confirm ok          ", csl_common_test_timer_v0(test_confirm_ok),"" );
  csl_common_print_results( "get ok              ", csl_common_test_timer_v0(test_get_ok),"" );
  csl_common_print_results( "start ok            ", csl_common_test_timer_v0(test_start_ok),"" );
  csl_common_print_results( "end ok              ", csl_common_test_timer_v0(test_end_ok),"" );
  csl_common_print_results( "flush ok            ", csl_common_test_timer_v0(test_flush_ok),"" );

  csl_common_print_results( "reserve failed      ", csl_common_test_timer_v0(test_reserve_failed),"" );
  csl_common_print_results( "confirm failed      ", csl_common_test_timer_v0(test_confirm_failed),"" );
  csl_common_print_results( "get failed          ", csl_common_test_timer_v0(test_get_failed),"" );
  csl_common_print_results( "start failed        ", csl_common_test_timer_v0(test_start_failed),"" );
  csl_common_print_results( "end failed          ", csl_common_test_timer_v0(test_end_failed),"" );
  csl_common_print_results( "flush failed        ", csl_common_test_timer_v0(test_flush_failed),"" );
  
#endif
  return 0;
}

/* EOF */

