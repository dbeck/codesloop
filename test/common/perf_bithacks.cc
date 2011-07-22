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
#include <vector>

using csl::common::bithacks;

namespace test_bithacks
{
#define ITER 1000000
  void malloc_baseline()
  {
    void * p = 0;
    for(int i=0;i<ITER;++i)
    {
      p = malloc(((i+1)<<2));
      free(p);
    }
  }

  void malloc2_baseline()
  {
    void * p[ITER];
    for(int i=0;i<ITER;++i)
    {
      p[i] = malloc(((i+1)<<2));
    }
    for(int i=0;i<ITER;++i)
    {
      free(p[i]);
    }
  }

  void malloc3_baseline()
  {
    void * p[ITER];
    for(int i=0;i<ITER;++i)
    {
      p[i] = malloc(((i+1)<<2));
      if( i>0 ) free(p[i-1]);
    }
    free(p[0]);
  }

  void vector_baseline()
  {
    std::vector<int> v;
    for(int i=0;i<ITER;++i)
    {
      v.push_back(i);
    }
  }

  void nlz_gcc()
  {
    static unsigned int g = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::nlz_gcc(g++);
    }
    g += j;
  }

  void ntz_gcc()
  {
    static unsigned int g2 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::ntz_gcc(g2++);
    }
    g2 += j;
  }

  void nlz_c1()
  {
    static unsigned int c = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::nlz_c1(c++);
    }
    c += j;
  }

  void ntz_c1()
  {
    static unsigned int c2 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::ntz_c1(c2++);
    }
    c2 += j;
  }


  void nlz_c2()
  {
    static unsigned int d = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::nlz_c2(d++);
    }
    d += j;
  }

  void ntz_c2()
  {
    static unsigned int t2 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::ntz_c2(t2++);
    }
    t2 += j;
  }

  void ntz_c3()
  {
    static unsigned int t3 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::ntz_c3(t3++);
    }
    t3 += j;
  }

  void ntz_c4()
  {
    static unsigned int t4 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::ntz_c4(t4++);
    }
    t4 += j;
  }

  void pop_gcc()
  {
    static unsigned int p0 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::pop_gcc(p0++);
    }
    p0 += j;
  }

  void pop_c1()
  {
    static unsigned int p1 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::pop_c1(p1++);
    }
    p1 += j;
  }

  void pop_c2()
  {
    static unsigned int p2 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::pop_c2(p2++);
    }
    p2 += j;
  }

  void pop_c3()
  {
    static unsigned int p3 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::pop_c3(p3++);
    }
    p3 += j;
  }

  void pop_c4()
  {
    static unsigned int p4 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::pop_c4(p4++);
    }
    p4 += j;
  }

  void pop_c5()
  {
    static unsigned int p5 = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::pop_c5(p5++);
    }
    p5 += j;
  }

  void bestfit_c1()
  {
    static unsigned int f1 = 0;
    int k,j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::bestfit_c1(f1++,1+(f1%31),&k);
    }
    f1 += j;
  }

  void firstfit_c1()
  {
    static unsigned int xf1 = 0;
    int k,j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::firstfit_c1(xf1++,1+(xf1%31),&k);
    }
    xf1 += j;
  }

  void last0bit()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::last0bit(q);
    }
    q += j;
  }

  void last0str()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::last0str(q);
    }
    q += j;
  }

  void last1bit()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::last1bit(q);
    }
    q += j;
  }

  void last1str()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::last1str(q);
    }
    q += j;
  }

  void last1bitoff()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::last1bitoff(q);
    }
    q += j;
  }

  void trailing0str()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += bithacks::trailing0str(q);
    }
    q += j;
  }

  void trailing0strB()
  {
    static unsigned int q = 0;
    int j = 0;
    for(int i=0;i<ITER;++i)
    {
      j += ((~q)&(q-1));
    }
    q += j;
  }

};

using namespace test_bithacks;

int main()
{
  csl_common_print_results( "malloc_baseline  ", csl_common_test_timer_v0(malloc_baseline),"" );
  csl_common_print_results( "malloc2_baseline ", csl_common_test_timer_v0(malloc2_baseline),"" );
  csl_common_print_results( "malloc3_baseline ", csl_common_test_timer_v0(malloc3_baseline),"" );
  csl_common_print_results( "vector_baseline  ", csl_common_test_timer_v0(vector_baseline),"" );
  csl_common_print_results( "ntz_gcc          ", csl_common_test_timer_v0(ntz_gcc),"" );
  csl_common_print_results( "ntz_c1           ", csl_common_test_timer_v0(ntz_c1),"" );
  csl_common_print_results( "ntz_c2           ", csl_common_test_timer_v0(ntz_c2),"" );
  csl_common_print_results( "ntz_c3           ", csl_common_test_timer_v0(ntz_c3),"" );
  csl_common_print_results( "ntz_c4           ", csl_common_test_timer_v0(ntz_c4),"" );
  csl_common_print_results( "nlz_gcc          ", csl_common_test_timer_v0(nlz_gcc),"" );
  csl_common_print_results( "nlz_c1           ", csl_common_test_timer_v0(nlz_c1),"" );
  csl_common_print_results( "nlz_c2           ", csl_common_test_timer_v0(nlz_c2),"" );
  csl_common_print_results( "pop_c1           ", csl_common_test_timer_v0(pop_c1),"" );
  csl_common_print_results( "pop_c2           ", csl_common_test_timer_v0(pop_c2),"" );
  csl_common_print_results( "pop_c3           ", csl_common_test_timer_v0(pop_c3),"" );
  csl_common_print_results( "pop_c4           ", csl_common_test_timer_v0(pop_c4),"" );
  csl_common_print_results( "pop_c5           ", csl_common_test_timer_v0(pop_c5),"" );
  csl_common_print_results( "pop_gcc          ", csl_common_test_timer_v0(pop_gcc),"" );
  csl_common_print_results( "bestfit_c1       ", csl_common_test_timer_v0(bestfit_c1),"" );
  csl_common_print_results( "firstfit_c1      ", csl_common_test_timer_v0(firstfit_c1),"" );
  csl_common_print_results( "last0bit         ", csl_common_test_timer_v0(last0bit),"" );
  csl_common_print_results( "last0str         ", csl_common_test_timer_v0(last0str),"" );
  csl_common_print_results( "last1bit         ", csl_common_test_timer_v0(last1bit),"" );
  csl_common_print_results( "last1str         ", csl_common_test_timer_v0(last1str),"" );
  csl_common_print_results( "last1bitoff      ", csl_common_test_timer_v0(last1bitoff),"" );
  csl_common_print_results( "trailing0str     ", csl_common_test_timer_v0(trailing0str),"" );
  csl_common_print_results( "trailing0strB    ", csl_common_test_timer_v0(trailing0strB),"" );

  return 0;
}

// EOF : perf_bithacks.cc
