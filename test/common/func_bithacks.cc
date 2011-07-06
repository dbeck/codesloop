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
  void check_fun_3(int(*fun1)(unsigned int),
                   int(*fun2)(unsigned int),
                   int(*fun3)(unsigned int),
                   unsigned int & in)
  {
    assert( (fun1(in) == fun2(in)) && (fun2(in)== fun3(in)) );
  }

  void check_fun_5(int(*fun1)(unsigned int),
                   int(*fun2)(unsigned int),
                   int(*fun3)(unsigned int),
                   int(*fun4)(unsigned int),
                   int(*fun5)(unsigned int),
                   unsigned int & in)
  {
    assert( (fun1(in) == fun2(in)) && (fun3(in) == fun4(in)) && (fun2(in) == fun5(in)) && (fun3(in) == fun1(in)) );
  }

  void loop1()
  {
    static unsigned int z1 = 0;
    check_fun_3(bithacks::nlz_gcc,
                bithacks::nlz_c1,
                bithacks::nlz_c2,
                z1);
    check_fun_5(bithacks::pop_gcc,
                bithacks::pop_c1,
                bithacks::pop_c2,
                bithacks::pop_c3,
                bithacks::pop_c4,
                z1);
    check_fun_5(bithacks::ntz_gcc,
                bithacks::ntz_c1,
                bithacks::ntz_c2,
                bithacks::ntz_c3,
                bithacks::ntz_c4,
                z1);
    z1++;
  }

  void loop2()
  {
    static unsigned int z2 = 0;
    check_fun_3(bithacks::nlz_gcc,
                bithacks::nlz_c1,
                bithacks::nlz_c2,
                z2);
    check_fun_5(bithacks::pop_gcc,
                bithacks::pop_c1,
                bithacks::pop_c2,
                bithacks::pop_c3,
                bithacks::pop_c4,
                z2);
    check_fun_5(bithacks::ntz_gcc,
                bithacks::ntz_c1,
                bithacks::ntz_c2,
                bithacks::ntz_c3,
                bithacks::ntz_c4,
                z2);
    z2 += 3;
  }

  void loop3()
  {
    static unsigned int z3 = 0;
    check_fun_3(bithacks::nlz_gcc,
                bithacks::nlz_c1,
                bithacks::nlz_c2,
                z3);
    check_fun_5(bithacks::pop_gcc,
                bithacks::pop_c1,
                bithacks::pop_c2,
                bithacks::pop_c3,
                bithacks::pop_c4,
                z3);
    check_fun_5(bithacks::ntz_gcc,
                bithacks::ntz_c1,
                bithacks::ntz_c2,
                bithacks::ntz_c3,
                bithacks::ntz_c4,
                z3);
    z3 += 119;
  }

};

using namespace test_bithacks;

int main()
{
  csl_common_print_results( "loop1 ", csl_common_test_timer_v0(loop1),"" );
  csl_common_print_results( "loop2 ", csl_common_test_timer_v0(loop2),"" );
  csl_common_print_results( "loop3 ", csl_common_test_timer_v0(loop3),"" );
  return 0;
}

// EOF : func_bithacks.cc
