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
#include "codesloop/common/zero.hh"

namespace csl
{
  namespace common
  {
    template <> const char                zero<char>::val_                = 0;
    template <> const wchar_t             zero<wchar_t>::val_             = 0;
    template <> const unsigned char       zero<unsigned char>::val_       = 0;
    template <> const short               zero<short>::val_               = 0;
    template <> const unsigned short      zero<unsigned short>::val_      = 0;
    template <> const int                 zero<int>::val_                 = 0;
    template <> const unsigned int        zero<unsigned int>::val_        = 0;
    template <> const long                zero<long>::val_                = 0;
    template <> const unsigned long       zero<unsigned long>::val_       = 0;
    template <> const long long           zero<long long>::val_           = 0;
    template <> const unsigned long long  zero<unsigned long long>::val_  = 0;
    template <> const float               zero<float>::val_               = 0.0;
    template <> const double              zero<double>::val_              = 0.0;
    template <> const long double         zero<long double>::val_         = 0.0;
  }
}

// EOF

