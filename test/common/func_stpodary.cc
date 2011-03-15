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

#if 0
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
//#define DEBUG_VERBOSE
#endif /* DEBUG */
#endif

#include "codesloop/common/stpodary.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include <assert.h>

using csl::common::stpodary;

namespace test_stpodary
{
  void empty_checks()
  {
    stpodary<char,10> t;
    assert( t.has_data() == false );
    assert( t.is_dynamic() == false );
    assert( t.size() == 0 );
    assert( t.nbytes() == 0 );
    // should always have some stack based space allocated
    assert( t.data() != 0 );
    assert( t.private_data() != 0 );
  }

  void copy_checks() { }
  void append_checks() { }
  void setter_checks() { }
  void getter_checks() { }
  void equality_checks() { }
  void allocate_checks() { }
  void reset_checks() { }


} // end of test_stpodary

using namespace test_stpodary;

int main()
{
  empty_checks();
  copy_checks();
  append_checks();
  setter_checks();
  getter_checks();
  equality_checks();
  allocate_checks();
  reset_checks();

  return 0;
}

/* EOF */
