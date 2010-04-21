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
//#define DEBUG_VERBOSE
#endif /* DEBUG */
//#endif

#include "codesloop/common/stream_nop_target.hh"
#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include <assert.h>

using namespace csl::common;

namespace test_stream {

  void test_baseline_nop_target()
  {
    stream_nop_target<char> t;
  }

  void test_baseline_buffer()
  {
    stream_buffer<char> b;
  }

  void test_baseline_stream()
  {
    stream_buffer<double> b;
    stream<double> s(b);
  }
  
  void test_flags()
  {
    stream_buffer<uint32_t> b;
    typedef stream<uint32_t> ui32stream_t;
    ui32stream_t s(b);
    // initial flags are ok_
    assert( s.flags() == stream_flags::ok_ );
        
    // get from empty buffer
    ui32stream_t::part_t p;
    ui32stream_t::part_t & sp(s.get(10,p));
    
    assert( p.flags() & stream_flags::empty_buffer_ );
    assert( sp.flags() & stream_flags::empty_buffer_ );
    
  }

}

using namespace test_stream;

int main()
{
#ifdef DEBUG
  test_flags();
  test_baseline_stream();
#else
  csl_common_print_results( "baseline nop target ", csl_common_test_timer_v0(test_baseline_nop_target),"" );
  csl_common_print_results( "baseline buffer     ", csl_common_test_timer_v0(test_baseline_buffer),"" );
  csl_common_print_results( "baseline stream     ", csl_common_test_timer_v0(test_baseline_stream),"" );
  csl_common_print_results( "flags               ", csl_common_test_timer_v0(test_flags),"" );
#endif
  return 0;
}

/* EOF */

