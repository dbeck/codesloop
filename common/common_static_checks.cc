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
#include "codesloop/common/stpodary.hh"
#include "codesloop/common/simpstr.hh"
#include "codesloop/common/zero.hh"
#include "codesloop/common/strlength.hh"

namespace
{
  static void static_check_simpstr()
  {
    csl::common::simpstr s;
  }
  
  static bool static_check_strlength()
  {
    const char    * c = "Hello world";
    const wchar_t * w = L"Hello world";
    
    csl::common::strlength<char>     charlen;
    csl::common::strlength<wchar_t>  wcharlen;
    
    size_t lc1 = csl::common::strlength<char>::execute(c);
    size_t lc2 = charlen(c);
    size_t lw1 = csl::common::strlength<wchar_t>::execute(w);
    size_t lw2 = wcharlen(w);
    
    return (lc1 == lc2 && lw1 == lw2);
  }
    
  class static_check_init
  {
  public:
    static_check_init()
    {
      static_check_simpstr();
      static_check_strlength();
    }
    
    csl::common::zero<char>       char_zero_;
    csl::common::zero<wchar_t>    wchar_zero_;
    csl::common::zero<int>        int_zero_;
    csl::common::zero<int64_t>    int64_zero_;
    csl::common::zero<float>      float_zero_;
    csl::common::zero<double>     double_zero_;
    csl::common::zero<long>       long_zero_;
    csl::common::zero<long long>  llong_zero_;
  
  };
  
  static static_check_init _static_check_init__;
}


// EOF

