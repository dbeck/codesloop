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

#include "codesloop/common/common.h"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/bithacks.hh"

using csl::common::bithacks;

namespace test_bithacks
{
  void nlz_gcc()
  {
    static unsigned int g = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::n_leading_zero_gcc(g++);
    }
    g += j;
  }

  void nlz_c1()
  {
    static unsigned int c = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::n_leading_zero_c1(c++);
    }
    c += j;
  }

  void nlz_c2()
  {
    static unsigned int d = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::n_leading_zero_c2(d++);
    }
    d += j;
  }

  void pop_gcc()
  {
    static unsigned int p0 = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::popcount_gcc(p0++);
    }
    p0 += j;
  }

  void pop_c1()
  {
    static unsigned int p1 = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::popcount_c1(p1++);
    }
    p1 += j;
  }

  void pop_c2()
  {
    static unsigned int p2 = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::popcount_c2(p2++);
    }
    p2 += j;
  }

  void pop_c3()
  {
    static unsigned int p3 = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::popcount_c3(p3++);
    }
    p3 += j;
  }

  void pop_c4()
  {
    static unsigned int p4 = 0;
    int j = 0;
    for(int i=0;i<6130;++i)
    {
      j += bithacks::popcount_c4(p4++);
    }
    p4 += j;
  }


};

using namespace test_bithacks;

int main()
{
  csl_common_print_results( "nlz_c1       ", csl_common_test_timer_v0(nlz_c1),"" );
  csl_common_print_results( "nlz_c2       ", csl_common_test_timer_v0(nlz_c2),"" );
  csl_common_print_results( "nlz_gcc      ", csl_common_test_timer_v0(nlz_gcc),"" );
  csl_common_print_results( "pop_c1       ", csl_common_test_timer_v0(pop_c1),"" );
  csl_common_print_results( "pop_c2       ", csl_common_test_timer_v0(pop_c2),"" );
  csl_common_print_results( "pop_c3       ", csl_common_test_timer_v0(pop_c3),"" );
  csl_common_print_results( "pop_c4       ", csl_common_test_timer_v0(pop_c4),"" );
  csl_common_print_results( "pop_gcc      ", csl_common_test_timer_v0(pop_gcc),"" );

  return 0;
}

// EOF : perf_bithacks.cc
