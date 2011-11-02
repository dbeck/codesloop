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
#include "codesloop/common/bufmgr.hh"
#include "codesloop/common/ksmsg.hh"
#include "codesloop/common/ksbuf.hh"
#include <memory>

using namespace csl::common;

namespace test_bufmgr
{
  void baseline2(char * c) { delete [] c; }
  void baseline1(char * c) { baseline2(c); }
  void baseline() { baseline1(new char[4096]); }

  void cbaseline2(void * c) { free(c); }
  void cbaseline1(void * c) { cbaseline2(c); }
  void cbaseline() { cbaseline1(malloc(4096)); }

  void bsitem1(bufmgr::item i) {}
  void bsitem2(bufmgr::item i) { bsitem1(std::move(i)); }
  void bsitem() { static bufmgr b; bsitem2(b.alloc()); }

  void bsitem3(bufmgr::item i) { bsitem2(std::move(i)); }
  void bsitem4(bufmgr::item i) { bsitem3(std::move(i)); }
  void bsitemb() { static bufmgr b; bsitem4(b.alloc()); }

  void ksitem2(ksmsg i)
  {
    scoped_kspin_lock lck(i.get_lock());
  }
  void ksitem1(ksmsg i) { ksitem2(i); }
  void ksitem()
  {
    static ksbuf<512,4096> b;
    ksbuf<512,4096>::result r(b.get());
    ksmsg::buf bf(r);
    // update size here ...
    ksmsg m(bf,r.get_lock());
    // or later under locking
    ksitem1(m);
  }
};

using namespace test_bufmgr;

int main()
{
  std::auto_ptr<bufmgr> b;
  csl_common_print_results( "baseline  ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "cbaseline ", csl_common_test_timer_v0(cbaseline),"" );
  csl_common_print_results( "bsitem    ", csl_common_test_timer_v0(bsitem),"" );
  csl_common_print_results( "ksitem    ", csl_common_test_timer_v0(ksitem),"" );
  csl_common_print_results( "bsitemb   ", csl_common_test_timer_v0(bsitemb),"" );
  return 0;
}

// EOF
