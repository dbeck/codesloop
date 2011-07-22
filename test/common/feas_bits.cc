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
#include "codesloop/common/bithacks.hh"

namespace test_bits
{
  void print(uint32_t x)
  {
    for(int i=0;i<32;++i)
    {
      printf("%s",(((x>>(31-i))&1)==1?"1":"0"));
    }
  }
  void e()
  {
    printf("\n");
  }
  void print(uint32_t x, const char * s)
  {
    print(x); printf(" : %19s",s);
  }
};

using namespace test_bits;
using namespace csl::common;

int main()
{
  uint32_t xs[]={ 0x3, 0x30, 0x80, 0x0f, 0xf00, 0xf0f, 0x55, 0x5005, 0x5050, 0x330033, 0x33003300, 0xffffffff };
  uint32_t * xp = xs;

  e();

  #define P(EXPR,MSG) { print((EXPR),#EXPR); printf(" : %s\n",MSG); }

  while( *xp != 0xffffffff )
  {
    uint32_t x = *xp;
    P(x,                    "");
    P(-x,                   "");
    P(x-1,                  "");
    P(x+1,                  "");
    P(~(x-1),               "");
    P(~(x+1),               "");
    P(x&(-x),               "last 1 bit");
    P(x&(x-1),              "turn off rigthmost bit");
    P(x&(x+1),              "2^n-1 if == 0");
    P(x^(-x),               "");
    P(x^(x-1),              "bits to first 1 bit incl");
    P(x^(x+1),              "bits to first 0 bit");
    P((~x)&(-x),            "");
    P((~x)&(x-1),           "trailing zeroes");
    P((~x)&(x+1),           "last 0 bit");
    P((~x)^(-x),            "bits to first 1 bit");
    P((~x)^(x-1),           "");
    P((~x)^(x+1),           "");
    P(x|(x-1),              "right fill bits");
    P(x|(x+1),              "set rightmost zero");
    P(x&((x|(x-1))+1),      "turn off rightmost 1-string");
    P(x^(x&((x|(x-1))+1)),  "rightmost 1-string");
    e();
    P(x,"");
    P(bithacks::last0bit(x),"");
    P(bithacks::last0str(x),"");
    P(bithacks::last1bit(x),"");
    P(bithacks::last1str(x),"");
    P(bithacks::last1bitoff(x),"");
    P(bithacks::trailing0str(x),"");
    e();
    ++xp;
  }

  e();

  return 0;
}

// EOF
