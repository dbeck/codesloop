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

#include "codesloop/comm/autofd.hh"
#include <assert.h>
#include <string.h>
#include <algorithm>

using csl::comm::autofd;

namespace test_autofd
{
  void copyop()
  {
    autofd a(2121);
    autofd b;
    b = std::move(a);
    assert( a.get() == -1 );
    assert( b.get() == 2121 );
    b = -1;
  }

  void copyconstr()
  {
    autofd a(2121);
    autofd b(std::move(a));
    assert( a.get() == -1 );
    assert( b.get() == 2121 );
    b = -1;
  }

  void pass(autofd a)
  {
    assert( a.get() == 2121 );
  }

  void passbyval()
  {
    autofd a(2121);
    pass(std::move(a));
    assert( a.get() == -1 );
  }

} // end of test_autofd

using namespace test_autofd;

int main()
{
  copyop();
  copyconstr();
  passbyval();
  return 0;
}

// EOF
