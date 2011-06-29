/*
Copyright (c) 2008,2009,2010,2011 CodeSLoop Team

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

#ifdef CSL_DEBUG
#define CSL_DEBUG_ENABLE_INDENT
#define CSL_DEBUG_VERBOSE
#endif /* CSL_DEBUG */

#include "codesloop/common/bithacks.hh"
#include "codesloop/common/test_timer.h"
#include <assert.h>

using csl::common::bithacks;

namespace test_bithacks
{
  void n_leading_zero_loop1()
  {
    static int i = 0;
    assert( bithacks::n_leading_zero_gcc(i) == bithacks::n_leading_zero_c1(i) );
    assert( bithacks::n_leading_zero_c2(i) == bithacks::n_leading_zero_c1(i) );
    ++i;
  }

  void n_leading_zero_loop2()
  {
    static int i2 = 0;
    assert( bithacks::n_leading_zero_gcc(i2) == bithacks::n_leading_zero_c1(i2) );
    assert( bithacks::n_leading_zero_c2(i2) == bithacks::n_leading_zero_c1(i2) );
    i2 += 3;
  }

  void n_leading_zero_loop3()
  {
    static int i3 = 0;
    assert( bithacks::n_leading_zero_gcc(i3) == bithacks::n_leading_zero_c1(i3) );
    assert( bithacks::n_leading_zero_c2(i3) == bithacks::n_leading_zero_c1(i3) );
    i3 += 119;
  }

};

using namespace test_bithacks;

int main()
{
  csl_common_print_results( "n_leading_zero_loop1 ", csl_common_test_timer_v0(n_leading_zero_loop1),"" );
  csl_common_print_results( "n_leading_zero_loop2 ", csl_common_test_timer_v0(n_leading_zero_loop2),"" );
  csl_common_print_results( "n_leading_zero_loop3 ", csl_common_test_timer_v0(n_leading_zero_loop3),"" );
  return 0;
}

// EOF : func_bithacks.cc
