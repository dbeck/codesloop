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
#include "codesloop/common/bitmap512.hh"
#include <vector>
#include <algorithm>

using csl::common::bitmap512;
using csl::common::excbase;

namespace test_bitmap512
{
  void baseline() { bitmap512 b; }
  void get_set_clear()
  {
    bitmap512 b;
    for(uint16_t i=0;i<512;++i)
    {
      if( b.get(i) != false ) { printf("\nERRA: %d %d",i,b.get(i)); }
      b.set(i);
      if( b.get(i) != true ) { printf("\nERR0: %d %d",i,b.get(i)); }
      b.clear(i);
      if( b.get(i) != false ) { printf("\nERR1: %d %d",i,b.get(i)); }
    }
  }
  void get_set_clear_v()
  {
    std::vector<bool> bv(512,false);
    //bv.reserve(512);
    for(uint16_t i=0;i<512;++i)
    {
      if( bv[i] != false ) { printf("\nERRB"); }
      bv[i] = true;
      if( bv[i] != true ) { printf("\nERR2"); }
      bv[i] = false;
      if( bv[i] != false ) { printf("\nERR3"); }
    }
  }
  void get_set_clear_x()
  {
    std::vector<uint8_t> bv(512,'F');
    //bv.reserve(512,'F');
    for(uint16_t i=0;i<512;++i)
    {
      if( bv[i] != 'F' ) { printf("\nERR4"); }
      bv[i] = 'T';
      if( bv[i] != 'T') { printf("\nERR5"); }
      bv[i] = 'F';
      if( bv[i] != 'F' ) { printf("\nERR6"); }
    }
  }
  void first_clear()
  {
    bitmap512 b;
    for(uint16_t i=0;i<511;++i)
    {
      b.set(i);
      if( b.first_clear() != (i+1) ) { printf("\nERR0"); }
    }
  }
  void first_clear_x()
  {
    std::vector<uint8_t> bv(512,'F');
    for(uint16_t i=0;i<511;++i)
    {
      bv[i] = 'T';
      if( (std::find(bv.begin(),bv.end(),'F')-bv.begin()) != i+1 ) { printf("\nERR5"); }
    }
  }
  void set_first()
  {
    bitmap512 b;
    for(uint16_t i=0;i<511;++i)
    {
      if( b.flip_first_clear() != i ) { printf("\nERR0"); }
    }
  }
  void set_first_x()
  {
    std::vector<uint8_t> bv(512,'F');
    for(uint16_t i=0;i<511;++i)
    {
      std::vector<uint8_t>::iterator it = std::find(bv.begin(),bv.end(),'F');
      *it = 'T';
      if( (it-bv.begin()) != i ) { printf("\nERR5"); }
    }
  }

};

using namespace test_bitmap512;

int main()
{
  csl_common_print_results( "first_clear      ", csl_common_test_timer_v0(first_clear),"" );
  csl_common_print_results( "first_clear_x    ", csl_common_test_timer_v0(first_clear_x),"" );

  csl_common_print_results( "set_first       ", csl_common_test_timer_v0(set_first),"" );
  csl_common_print_results( "set_first_x     ", csl_common_test_timer_v0(set_first_x),"" );

  csl_common_print_results( "baseline         ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "get_set_clear    ", csl_common_test_timer_v0(get_set_clear),"" );
  csl_common_print_results( "get_set_clear_v  ", csl_common_test_timer_v0(get_set_clear_v),"" );
  csl_common_print_results( "get_set_clear_x  ", csl_common_test_timer_v0(get_set_clear_x),"" );
  return 0;
}

// EOF : perf_bitmap512.cc
