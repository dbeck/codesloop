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
#include "codesloop/common/kspin.hh"
#include <assert.h>

using namespace csl::common;

namespace test_kspin
{
  void baseline()
  {
    kspin ks;
  }

  void xlock()
  {
    kspin ks;
    ks.xlock(kspin::init_,1);
  }

  void lock()
  {
    kspin ks;
    ks.lock(kspin::init_);
  }

  void unlock()
  {
    kspin ks;
    ks.unlock(1);
  }

  void lock_unlock()
  {
    kspin ks;
    ks.lock(kspin::init_);
    ks.unlock(1);
  }

  void validate()
  {
    kspin ks;
    assert(ks.load()==kspin::init_);
    ks.xlock(kspin::init_,1);
    {
      assert(ks.lock(1) == true);
      assert(ks.load()==0);
      ks.unlock(1);
      assert(ks.load()==1);
    }

    assert(ks.lock(2) == false);
    assert(ks.load()==1);

    {
      assert(ks.lock(1) == true);
      ks.unlock(1);
      assert(ks.load()==1);
    }
  }
}

using namespace test_kspin;

int main()
{
  validate();
  csl_common_print_results( "baseline                 ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "xlock                    ", csl_common_test_timer_v0(xlock),"" );
  csl_common_print_results( "lock                     ", csl_common_test_timer_v0(lock),"" );
  csl_common_print_results( "unlock                   ", csl_common_test_timer_v0(unlock),"" );
  csl_common_print_results( "lock-unlock              ", csl_common_test_timer_v0(lock_unlock),"" );
  return 0;
}
