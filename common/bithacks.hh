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
      static inline int nlz_gcc(unsigned int x)
      {
        if (x == 0) return(32);
        return __builtin_clz(x);
      }

      static inline int nlz_c1(unsigned int x)
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

      static inline int nlz_c2(unsigned int x)
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
#ifndef fastest_nlz
#define fastest_nlz nlz_gcc
#endif // fastest_nlz

      static inline int nlz(unsigned int x) { return fastest_nlz(x); }

      static inline int ntz_gcc(unsigned int x)
      {
        if(x==0) return(32);
        return __builtin_ctz(x);
      }

      static inline int ntz_c1(unsigned int x)
      {
        int n;

        if (x == 0) return(32);
        n = 1;
        if ((x & 0x0000FFFF) == 0) {n = n +16; x = x >>16;}
        if ((x & 0x000000FF) == 0) {n = n + 8; x = x >> 8;}
        if ((x & 0x0000000F) == 0) {n = n + 4; x = x >> 4;}
        if ((x & 0x00000003) == 0) {n = n + 2; x = x >> 2;}
        return n - (x & 1);
      }

      static inline int ntz_c2(unsigned int x)
      {
        unsigned y, bz, b4, b3, b2, b1, b0;

        y = x & -x;
        bz = y ? 0 : 1;
        b4 = (y & 0x0000FFFF) ? 0 : 16;
        b3 = (y & 0x00FF00FF) ? 0 : 8;
        b2 = (y & 0x0F0F0F0F) ? 0 : 4;
        b1 = (y & 0x33333333) ? 0 : 2;
        b0 = (y & 0x55555555) ? 0 : 1;
        return bz + b4 + b3 + b2 + b1 + b0;
      }

      static inline int ntz_c3(unsigned int x)
      {
        enum { u=99 };
        static char table[64] =
          {32, 0, 1,12, 2, 6, u,13,   3, u, 7, u, u, u, u,14,
           10, 4, u, u, 8, u, u,25,   u, u, u, u, u,21,27,15,
           31,11, 5, u, u, u, u, u,   9, u, u,24, u, u,20,26,
           30, u, u, u, u,23, u,19,  29, u,22,18,28,17,16, u};

        x = (x & -x)*0x0450FBAF;
        return table[x >> 26];
      }

      static inline int ntz_c4(unsigned int x)
      {
        enum { u=99 };
        static char table[37] =
          {32,  0,  1, 26,  2, 23, 27,
           u,  3, 16, 24, 30, 28, 11,  u, 13,  4,
           7, 17,  u, 25, 22, 31, 15, 29, 10, 12,
           6,  u, 21, 14,  9,  5, 20,  8, 19, 18};

        x = (x & -x)%37;
        return table[x];
      }

// select fastest
#ifndef fastest_ntz
#define fastest_ntz ntz_gcc
#endif // fastest_ntz

      static inline int ntz(unsigned int x) { return fastest_ntz(x); }

      static inline int pop_gcc(unsigned int x)
      {
        return __builtin_popcount(x);
      }

      static inline int pop_c1(unsigned int x)
      {
        x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
        x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
        x = (x & 0x0000FFFF) + ((x >>16) & 0x0000FFFF);
        return x;
      }

      static inline int pop_c2(unsigned int x)
      {
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return x & 0x0000003F;
      }

      static inline int pop_c3(unsigned int x)
      {
        int n = 0;
        while (x != 0)
        {
          n = n + 1;
          x = x & (x - 1);
        }
        return n;
      }

      static inline int pop_c4(unsigned int x)
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

      static inline int pop_c5(unsigned int x)
      {
        static char table[16] = { 0, 1, 1, 2, 1, 2, 2, 3,  1, 2, 2, 3, 2, 3, 3, 4 };

        return table[x         & 0xF] +
               table[(x >>  4) & 0xF] +
               table[(x >>  8) & 0xF] +
               table[(x >> 12) & 0xF] +
               table[(x >> 16) & 0xF] +
               table[(x >> 20) & 0xF] +
               table[(x >> 24) & 0xf] +
               table[(x >> 28) & 0xf];
      }

// select fastest pop
#ifndef fastest_pop
#define fastest_pop pop_c4
#endif // fastest_pop

      static inline int pop(unsigned int x) { return fastest_pop(x); }

      static inline int bestfit_c1(unsigned int x, int n, int *apos)
      {
        int k, kmin, y0, y;
        unsigned int x0, xmin;

        kmin = 32;
        xmin = x;
        y0 = pop(x);
        x0 = x;
        do
        {
          x = ((x | (x - 1)) + 1) & x;      // Turn off
          y = pop(x);               // rightmost string.
          k = y0 - y;                       // k = length of string
          if (k <= kmin && k >= n)          // turned off.
          {                                 //
            kmin = k;                       // Save shortest length
            xmin = x;                       // found, and the string.
          }
          y0 = y;
        } while (x != 0);
        *apos = nlz(x0 ^ xmin);
        return kmin;
      }

// select fastest bestfit
#ifndef fastest_bestfit
#define fastest_bestfit bestfit_c1
#endif // fastest_bestfit

      static inline int bestfit(unsigned int x, int n, int *apos)
      {
    	  return fastest_bestfit(x,n,apos);
      }

      static inline int firstfit_c1(unsigned int x, int n, int *apos)
      {
        int k, y0, kmin, y;
        unsigned int x0, xmin;

        xmin = x;
        y0 = pop(x);
        x0 = x;
        do
        {
          x = ((x | (x - 1)) + 1) & x;      // Turn off
          y = pop(x);               // rightmost string.
          k = y0 - y;                       // k = length of string
          if( k>=n )
          {
            kmin = k;
            xmin = x;
            break;
          }
          y0 = y;
        } while (x != 0);
        *apos = nlz(x0 ^ xmin);
        return kmin;
      }

// select fastest firstfit
#ifndef fastest_firstfit
#define fastest_firstfit firstfit_c1
#endif // fastest_firstfit

      static inline int firstfit(unsigned int x, int n, int *apos)
      {
        return fastest_firstfit(x,n,apos);
      }

      static inline unsigned int last0bit(unsigned int x)
      {
        // 00011 =>
        // 00100
        return ((~x)&(x+1));
      }

      static inline unsigned int last0str(unsigned int x)
      {
        // 00110011 =>
    	// 00001100
    	unsigned int y = ~x;
    	return (y^(y&((y|(y-1))+1)));
      }

      static inline unsigned int last1bit(unsigned int x)
      {
        // 101100000 =>
        // 000100000
        return (x&(-x));
      }

      static inline unsigned int last1str(unsigned int x)
      {
        // 101100000 =>
        // 001100000
        return (x^(x&((x|(x-1))+1)));
      }

      static inline unsigned int last1bitoff(unsigned int x)
      {
          // 00110011 =>
          // 00110010
        return (x&(x-1));
      }

      static inline unsigned int trailing0str(unsigned int x)
      {
        // 00110000 =>
        // 00001111
        return ((~x)&(x-1));
      }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_bithacks_hh_included_ */
