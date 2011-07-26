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

#include "codesloop/common/bitmap512.hh"

namespace csl
{
  namespace common
  {
    uint32_t bitmap512::empty_[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
    /*
    uint8_t bitmap512::empty_[64] =
    {
      0,0,0,0, 0,0,0,0,
      0,0,0,0, 0,0,0,0,
      0,0,0,0, 0,0,0,0,
      0,0,0,0, 0,0,0,0
    };

    uint8_t bitmap512::first_free_[256] =
    {
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,6,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,7,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,6,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
      0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,255
    };

    uint8_t bitmap512::max_free_[256] =
    {
      8,7,6,6,5,5,5,5, 4,4,4,4,4,4,4,4, 4,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3,
      5,4,3,3,2,2,2,2, 3,2,2,2,2,2,2,2, 4,3,2,2,2,2,2,2, 3,2,2,2,2,2,2,2,
      6,5,4,4,3,3,3,3, 3,2,2,2,2,2,2,2, 4,3,2,2,2,1,1,1, 3,2,1,1,2,1,1,1,
      5,4,3,3,2,2,2,2, 3,2,1,1,2,1,1,1, 4,3,2,2,2,1,1,1, 3,2,1,1,2,1,1,1,
      7,6,5,5,4,4,4,4, 3,3,3,3,3,3,3,3, 4,3,2,2,2,2,2,2, 3,2,2,2,2,2,2,2,
      5,4,3,3,2,2,2,2, 3,2,1,1,2,1,1,1, 4,3,2,2,2,1,1,1, 3,2,1,1,2,1,1,1,
      6,5,4,4,3,3,3,3, 3,2,2,2,2,2,2,2, 4,3,2,2,2,1,1,1, 3,2,1,1,2,1,1,1,
      5,4,3,3,2,2,2,2, 3,2,1,1,2,1,1,1, 4,3,2,2,2,1,1,1, 3,2,1,1,2,1,1,0
    };

    uint8_t bitmap512::max_pos_[256] =
    {
      0,1,2,2,3,3,3,3, 4,4,4,4,4,4,4,4, 0,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,
      0,1,2,2,6,6,6,6, 0,6,6,6,6,6,6,6, 0,1,6,6,6,6,6,6, 0,6,6,6,6,6,6,6,
      0,1,2,2,3,3,3,3, 0,4,4,4,4,4,4,4, 0,1,2,2,0,7,7,7, 0,1,7,7,0,7,7,7,
      0,1,2,2,3,3,3,3, 0,1,7,7,0,7,7,7, 0,1,2,2,0,7,7,7, 0,1,7,7,0,7,7,7,
      0,1,2,2,3,3,3,3, 4,4,4,4,4,4,4,4, 0,1,5,5,5,5,5,5, 0,5,5,5,5,5,5,5,
      0,1,2,2,3,3,3,3, 0,1,6,6,0,6,6,6, 0,1,2,2,0,6,6,6, 0,1,6,6,0,6,6,6,
      0,1,2,2,3,3,3,3, 0,4,4,4,4,4,4,4, 0,1,2,2,0,5,5,5, 0,1,5,5,0,5,5,5,
      0,1,2,2,3,3,3,3, 0,1,4,4,0,4,4,4, 0,1,2,2,0,3,3,3, 0,1,2,2,0,1,0,255
    };

    uint8_t bitmap512::max_posB_[256] =
    {
      0,1,2,2,3,3,3,3, 4,4,4,4,4,4,4,4, 0,1,5,5,5,5,5,5, 0,5,5,5,5,5,5,5,
      0,1,2,2,0,3,3,3, 0,1,6,6,0,6,6,6, 0,1,2,2,0,6,6,6, 0,1,6,6,0,6,6,6,
      0,1,2,2,3,3,3,3, 0,1,4,4,0,4,4,4, 0,1,2,2,0,1,0,3, 0,1,0,2,0,1,0,5,
      0,1,2,2,0,3,3,3, 0,1,0,2,0,1,0,4, 0,1,2,2,0,1,0,3, 0,1,0,2,0,1,0,7,
      0,1,2,2,3,3,3,3, 0,4,4,4,4,4,4,4, 0,1,2,2,0,5,5,5, 0,1,5,5,0,5,5,5,
      0,1,2,2,0,3,3,3, 0,1,0,2,0,1,0,4, 0,1,2,2,0,1,0,3, 0,1,0,2,0,1,0,6,
      0,1,2,2,3,3,3,3, 0,1,4,4,0,4,4,4, 0,1,2,2,0,1,0,3, 0,1,0,2,0,1,0,5,
      0,1,2,2,0,3,3,3, 0,1,0,2,0,1,0,4, 0,1,2,2,0,1,0,3, 0,1,0,2,0,1,0,255
    };
    */
  }
}

// EOF: bitmap512.cc
