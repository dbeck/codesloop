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
#include <atomic>

//using namespace csl::common;

namespace test_atomic
{
#ifdef CSL_GCC
  void atomic_xadd()
  {
    static unsigned int i = 0;
    __sync_fetch_and_add((&i),3);
  }

  void xadd()
  {
    static unsigned int i = 0;
    i += 3;
  }

  void atomic_inc()
  {
    static unsigned int i = 0;
    __sync_add_and_fetch((&i),3);
  }

  void inc()
  {
    static unsigned int i = 0;
    ++i;
  }

  void test_gcc()
  {
    csl_common_print_results( "atomic_xadd                 ", csl_common_test_timer_v0(atomic_xadd),"" );
    csl_common_print_results( "xadd                        ", csl_common_test_timer_v0(xadd),"" );
    csl_common_print_results( "atomic_inc                  ", csl_common_test_timer_v0(atomic_inc),"" );
    csl_common_print_results( "inc                         ", csl_common_test_timer_v0(inc),"" );
  }

#else /*CSL_GCC*/
  void test_gcc() { }
#endif /*CSL_GCC*/

  void test_atomic_bool()
  {
    static std::atomic_bool b;
    if( b ) b = false;
    else    b = true;
  }

  void test_atomic_load()
  {
    static std::atomic_bool i;
    bool ii = std::atomic_load( &i );
  }

  void test_atomic_load_store()
  {
    static std::atomic_bool i;
    bool ii = std::atomic_load( &i );
    ii = (ii ? false : true);
    std::atomic_store( &i, ii );
  }

  void test_atomic_load_explicit()
  {
    static std::atomic_bool i;
    bool ii = std::atomic_load_explicit( &i, std::memory_order_acquire );
  }

  void test_atomic_load_store_explicit()
  {
    static std::atomic_bool i;
    bool ii = std::atomic_load_explicit( &i, std::memory_order_acquire );
    ii = (ii ? false : true);
    std::atomic_store_explicit( &i, ii, std::memory_order_release );
  }

  void test_atomic_load_store_explicit2()
  {
    static std::atomic_bool i;
    bool ii = std::atomic_load_explicit( &i, std::memory_order_relaxed );
    ii = (ii ? false : true);
    std::atomic_store_explicit( &i, ii, std::memory_order_relaxed );
  }

  void test_std()
  {
    csl_common_print_results( "atomic bool                 ", csl_common_test_timer_v0(test_atomic_bool),"" );
    csl_common_print_results( "atomic load                 ", csl_common_test_timer_v0(test_atomic_load),"" );
    csl_common_print_results( "atomic load (acq)           ", csl_common_test_timer_v0(test_atomic_load_explicit),"" );
    csl_common_print_results( "atomic load store           ", csl_common_test_timer_v0(test_atomic_load_store),"" );
    csl_common_print_results( "atomic load store (acq,rel) ", csl_common_test_timer_v0(test_atomic_load_store_explicit),"" );
    csl_common_print_results( "atomic load store (rxd)     ", csl_common_test_timer_v0(test_atomic_load_store_explicit2),"" );
  }
}

using namespace test_atomic;

int main()
{
  test_gcc();
  test_std();
  return 0;
}

// EOF
