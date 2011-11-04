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
#include "codesloop/common/allocator.hh"
#include "codesloop/common/stpodary.hh"
#include <vector>
#include <map>
#include <list>
#include <limits>

using namespace csl::common;

namespace test_allocator
{
  template <typename T>
  void test(T & t, size_t res, size_t loop)
  {
    t.reserve(res);
    for(size_t i=0;i<loop;++i)
    {
      t.push_back(0);
    }
  }

  template <typename T>
  void test2(T & t, size_t res, size_t loop)
  {
    t.allocate(res);
    for(size_t i=0;i<loop;++i)
    {
      t.set_at(i,0);
    }
  }

  template <typename T>
  void test3(T & t, int loop)
  {
    for(int i=0;i<loop;++i)
    {
      t[i] = i;
    }
  }

  void baseline()         { std::vector<int> v;                 }
  void baseline_16()      { std::vector<int> v; v.reserve(16);  }
  void baseline_256()     { std::vector<int> v; v.reserve(256); }

  void map_std()          { std::map<int,int,std::less<int> >                                                    m; }
  void map_stdc()         { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::stdc> >       m; }
  void map_simpstack()    { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::simpstack> >  m; }

  void map_std_16()          { std::map<int,int,std::less<int> >                                                    m; test3(m,16); }
  void map_stdc_16()         { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::stdc> >       m; test3(m,16); }
  void map_simpstack_16()    { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::simpstack> >  m; test3(m,16); }

  void map_std_128()          { std::map<int,int,std::less<int> >                                                    m; test3(m,128);  }
  void map_stdc_128()         { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::stdc> >       m; test3(m,128);  }
  void map_simpstack_128()    { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::simpstack> >  m; test3(m,128);  }

  void map_std_1024()          { std::map<int,int,std::less<int> >                                                    m; test3(m,1024); }
  void map_stdc_1024()         { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::stdc> >       m; test3(m,1024); }
  void map_simpstack_1024()    { std::map<int,int,std::less<int>,allocator<std::pair<int,int>,256,impl::simpstack> >  m; test3(m,1024); }

  void vector_16_128()    { std::vector<int> v; test(v,16,128);    }
  void vector_256_512()   { std::vector<int> v; test(v,256,512);   }
  void vector_1024_1024() { std::vector<int> v; test(v,1024,1024); }
  void vector_1_1024()    { std::vector<int> v; test(v,1,1024);    }

  void vector2_16_128()    { std::vector<int> v(16);   test(v,16,128);    }
  void vector2_256_512()   { std::vector<int> v(256);  test(v,256,512);   }
  void vector2_1024_1024() { std::vector<int> v(1024); test(v,1024,1024); }
  void vector2_1_1024()    { std::vector<int> v(1024); test(v,1,1024);    }

  void baseline_stdc()      { std::vector<int, allocator<int,1> > v;                   }
  void baseline_16_stdc()   { std::vector<int, allocator<int,16> > v; v.reserve(16);   }
  void baseline_256_stdc()  { std::vector<int, allocator<int,256> > v; v.reserve(256); }

  void stdc_16_128()     { std::vector<int, allocator<int,16> >   v; test(v,16,128);    }
  void stdc_256_512()    { std::vector<int, allocator<int,256> >  v; test(v,256,512);   }
  void stdc_1024_1024()  { std::vector<int, allocator<int,1024> > v; test(v,1024,1024); }
  void stdc_1_1024()     { std::vector<int, allocator<int> >      v; test(v,1,1024);    }

  void baseline_simpstack()      { std::vector<int, allocator<int,1,impl::simpstack > >   v;                 }
  void baseline_16_simpstack()   { std::vector<int, allocator<int,16,impl::simpstack > >  v; v.reserve(16);  }
  void baseline_256_simpstack()  { std::vector<int, allocator<int,256,impl::simpstack > > v; v.reserve(256); }

  void simpstack_16_128()     { std::vector<int, allocator<int,16,impl::simpstack   > >   v; test(v,16,128);    }
  void simpstack_256_512()    { std::vector<int, allocator<int,256,impl::simpstack  > >   v; test(v,256,512);   }
  void simpstack_1024_1024()  { std::vector<int, allocator<int,1024,impl::simpstack > >   v; test(v,1024,1024); }
  void simpstack_1_1024()     { std::vector<int, allocator<int,256,impl::simpstack    > > v; test(v,1,1024);    }

  void stpodary_16_128()     { stpodary<int, 16>    v; test2(v,16,128);    }
  void stpodary_256_512()    { stpodary<int, 256>   v; test2(v,256,512);   }
  void stpodary_1024_1024()  { stpodary<int, 1024>  v; test2(v,1024,1024); }
  void stpodary_1_1024()     { stpodary<int, 256>   v; test2(v,1,1024);    }


} // test_allocator

using namespace test_allocator;

int main()
{
  csl_common_print_results( "map std     baseline ", csl_common_test_timer_v0(map_std),"" );
  csl_common_print_results( "map stdc    baseline ", csl_common_test_timer_v0(map_stdc),"" );
  csl_common_print_results( "map simpst  baseline ", csl_common_test_timer_v0(map_simpstack),"" );

  csl_common_print_results( "map std           16 ", csl_common_test_timer_v0(map_std_16),"" );
  csl_common_print_results( "map stdc          16 ", csl_common_test_timer_v0(map_stdc_16),"" );
  csl_common_print_results( "map simpstack     16 ", csl_common_test_timer_v0(map_simpstack_16),"" );

  csl_common_print_results( "map std          128 ", csl_common_test_timer_v0(map_std_128),"" );
  csl_common_print_results( "map stdc         128 ", csl_common_test_timer_v0(map_stdc_128),"" );
  csl_common_print_results( "map simpstack    128 ", csl_common_test_timer_v0(map_simpstack_128),"" );

  csl_common_print_results( "map std         1024 ", csl_common_test_timer_v0(map_std_1024),"" );
  csl_common_print_results( "map stdc        1024 ", csl_common_test_timer_v0(map_stdc_1024),"" );
  csl_common_print_results( "map simpstack   1024 ", csl_common_test_timer_v0(map_simpstack_1024),"" );

  csl_common_print_results( "vector      baseline ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "stdc        baseline ", csl_common_test_timer_v0(baseline_stdc),"" );
  csl_common_print_results( "simstack    baseline ", csl_common_test_timer_v0(baseline_simpstack),"" );

  csl_common_print_results( "16_vector   baseline ", csl_common_test_timer_v0(baseline_16),"" );
  csl_common_print_results( "16_stdc     baseline ", csl_common_test_timer_v0(baseline_16_stdc),"" );
  csl_common_print_results( "16_simstack baseline ", csl_common_test_timer_v0(baseline_16_simpstack),"" );

  csl_common_print_results( "256_vector    bsline ", csl_common_test_timer_v0(baseline_256),"" );
  csl_common_print_results( "256_stdc      bsline ", csl_common_test_timer_v0(baseline_256_stdc),"" );
  csl_common_print_results( "256_simstack  bsline ", csl_common_test_timer_v0(baseline_256_simpstack),"" );

  csl_common_print_results( "vector        1:1024 ", csl_common_test_timer_v0(vector_1_1024),"" );
  csl_common_print_results( "vector 2      1:1024 ", csl_common_test_timer_v0(vector2_1_1024),"" );
  csl_common_print_results( "stdc          1:1204 ", csl_common_test_timer_v0(stdc_1_1024),"" );
  csl_common_print_results( "simstack      1:1024 ", csl_common_test_timer_v0(simpstack_1_1024),"" );
  csl_common_print_results( "stpodary      1:1024 ", csl_common_test_timer_v0(stpodary_1_1024),"" );

  csl_common_print_results( "vector     1024:1024 ", csl_common_test_timer_v0(vector_1024_1024),"" );
  csl_common_print_results( "vector 2   1024:1024 ", csl_common_test_timer_v0(vector2_1024_1024),"" );
  csl_common_print_results( "stdc       1024:1024 ", csl_common_test_timer_v0(stdc_1024_1024),"" );
  csl_common_print_results( "simstack   1024:1024 ", csl_common_test_timer_v0(simpstack_1024_1024),"" );
  csl_common_print_results( "stpodary   1024:1024 ", csl_common_test_timer_v0(stpodary_1024_1024),"" );

  csl_common_print_results( "vector       256:512 ", csl_common_test_timer_v0(vector_256_512),"" );
  csl_common_print_results( "vector 2     256:512 ", csl_common_test_timer_v0(vector2_256_512),"" );
  csl_common_print_results( "stdc         256:512 ", csl_common_test_timer_v0(stdc_256_512),"" );
  csl_common_print_results( "simstack     256:512 ", csl_common_test_timer_v0(simpstack_256_512),"" );
  csl_common_print_results( "stpodary     256:512 ", csl_common_test_timer_v0(stpodary_256_512),"" );

  csl_common_print_results( "vector        16:128 ", csl_common_test_timer_v0(vector_16_128),"" );
  csl_common_print_results( "vector 2      16:128 ", csl_common_test_timer_v0(vector2_16_128),"" );
  csl_common_print_results( "stdc          16:128 ", csl_common_test_timer_v0(stdc_16_128),"" );
  csl_common_print_results( "stpodary      16:128 ", csl_common_test_timer_v0(stpodary_16_128),"" );

  return 0;
}

// EOF
