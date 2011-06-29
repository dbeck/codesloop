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

#ifndef _csl_common_bithacks_hh_included_
#define _csl_common_bithacks_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/dbc.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    struct bithacks
    {
      static inline int n_leading_zero_gcc(unsigned int x)
      {
        if (x == 0) return(32);
        return __builtin_clz(x);
      }

      static inline int n_leading_zero_c1(unsigned int x)
      {
        int n;
        if (x == 0) return(32);
        n = 0;
        if (x <= 0x0000FFFF) {n = n +16; x = x <<16;}
        if (x <= 0x00FFFFFF) {n = n + 8; x = x << 8;}
        if (x <= 0x0FFFFFFF) {n = n + 4; x = x << 4;}
        if (x <= 0x3FFFFFFF) {n = n + 2; x = x << 2;}
        if (x <= 0x7FFFFFFF) {n = n + 1;}
        return n;
      }

      static inline int n_leading_zero_c2(unsigned int x)
      {
        enum { u=99 };
         static char table[64] =
           {32,31, u,16, u,30, 3, u,  15, u, u, u,29,10, 2, u,
             u, u,12,14,21, u,19, u,   u,28, u,25, u, 9, 1, u,
            17, u, 4, u, u, u,11, u,  13,22,20, u,26, u, u,18,
             5, u, u,23, u,27, u, 6,   u,24, 7, u, 8, u, 0, u};

        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        x = (x << 3) - x;
        x = (x << 8) - x;
        x = (x << 8) - x;
        x = (x << 8) - x;
        return table[x >> 26];
      }

// select fastest
#ifndef n_leading_zero
#define n_leading_zero n_leading_zero_gcc
#endif // n_leading_zero

      static inline int popcount_gcc(unsigned int x)
      {
        return __builtin_popcount(x);
      }

      static inline int popcount_c1(unsigned int x)
      {
        x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
        x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
        x = (x & 0x0000FFFF) + ((x >>16) & 0x0000FFFF);
        return x;
      }

      static inline int popcount_c2(unsigned int x)
      {
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return x & 0x0000003F;
      }

      static inline int popcount_c3(unsigned int x)
      {
        int n = 0;
        while (x != 0)
        {
          n = n + 1;
          x = x & (x - 1);
        }
        return n;
      }

      static inline int popcount_c4(unsigned int x)
      {
        static char table[256] = {
          0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
          1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
          1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
          2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,

          1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
          2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
          2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
          3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,

           1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
           2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
           2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
           3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,

           2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
           3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
           3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
           4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

        return table[x         & 0xFF] +
               table[(x >>  8) & 0xFF] +
               table[(x >> 16) & 0xFF] +
               table[(x >> 24)];
      }

// select fastest
#ifndef popcount
#define popcount popcount_c2
#endif // popcount

    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_bithacks_hh_included_ */
