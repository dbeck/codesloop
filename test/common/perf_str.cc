/*
   Copyright (c) 2008,2009,2010, CodeSLoop Team

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

#include "codesloop/common/str.hh"
#include "codesloop/common/excbase.hh"
#include "codesloop/common/test_timer.h"
#include <assert.h>
#include <sys/stat.h>

using namespace csl::common;

namespace test_str {

  void string_baseline() { std::string b; }
  void str_baseline()    { str b; }

  void string_hello()
  {
    std::string b;
    b = "Hello";
  }

  void str_hello()
  {
    str b;
    b = L"Hello";
  }

  void string_concat()
  {
    std::string b;
    b = "Hello" + std::string("world");
  }

  void str_concat()
  {
    str b;
    b = L"Hello" + str(L"world");
  }

  void string_append()
  {
    std::string b;
    b += "Hello";
    b += " ";
    b += "world!";
  }

  void str_append()
  {
    str b;
    b += L"Hello";
    b += L" ";
    b += L"world!";
  }

  void string_initconstr() { std::string b("hello world"); }
  void str_initconstr()    { str b(L"hello world"); }

  void string_copy() { std::string b("hello world"); std::string c(b); }
  void str_copy()    { str b(L"hello world");        str c(b);     }

  void string_copymod() { std::string b("hello world"); std::string c(b); b += "!";  }
  void str_copymod()    { str b(L"hello world");        str c(b);         b += L"!"; }


} // namespace test_str


using namespace test_str;

int main()
{
  const char * strs[] = { "LC_ALL", "LANG", "LC_CTYPE", "LC_MESSAGES", 0 };
  const char ** strp = strs;

  while( *strp )
  {
    if( getenv(*strp) == 0 )
    {
      fprintf(stderr,"Warning '%s' has not been set\n",*strp);
    }
    ++strp;
  }

  if (!setlocale(LC_CTYPE, ""))
  {
    fprintf(stderr, "Can't set the specified locale! Check LANG, LC_CTYPE, LC_ALL.\n");
    exit(-1);
  }

  csl_common_print_results( "string_baseline    ", csl_common_test_timer_v0(string_baseline),"" );
  csl_common_print_results( "str_baseline       ", csl_common_test_timer_v0(str_baseline),"" );

  csl_common_print_results( "string_hello       ", csl_common_test_timer_v0(string_hello),"" );
  csl_common_print_results( "str_hello          ", csl_common_test_timer_v0(str_hello),"" );

  csl_common_print_results( "string_concat      ", csl_common_test_timer_v0(string_concat),"" );
  csl_common_print_results( "str_concat         ", csl_common_test_timer_v0(str_concat),"" );

  csl_common_print_results( "string_append      ", csl_common_test_timer_v0(string_append),"" );
  csl_common_print_results( "str_append         ", csl_common_test_timer_v0(str_append),"" );

  csl_common_print_results( "string_initconstr  ", csl_common_test_timer_v0(string_initconstr),"" );
  csl_common_print_results( "str_initconstr     ", csl_common_test_timer_v0(str_initconstr),"" );


  csl_common_print_results( "string_copy        ", csl_common_test_timer_v0(string_copy),"" );
  csl_common_print_results( "str_copy           ", csl_common_test_timer_v0(str_copy),"" );


  csl_common_print_results( "string_copymod     ", csl_common_test_timer_v0(string_copymod),"" );
  csl_common_print_results( "str_copymod        ", csl_common_test_timer_v0(str_copymod),"" );

  return 0;
}

// EOF

