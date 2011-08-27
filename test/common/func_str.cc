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

#include "codesloop/common/str.hh"
#include "codesloop/common/excbase.hh"
#include "codesloop/common/zero.hh"
#include <string>
#include <assert.h>

using csl::common::str;
using csl::common::excbase;
using csl::common::zero;

#ifndef ASSERT_OR_EXC
#define ASSERT_OR_EXC(EXPR)                       \
    do {                                          \
      bool caught__ = false;                      \
      try {                                       \
        assert( (EXPR) );                         \
      } catch(  csl::common::excbase & exc__ ) {  \
        caught__ = true;                          \
        print_exc_caught( exc__ );                \
      }                                           \
      CSL_DEBUG_ASSERT( caught__ == true );       \
    } while( 0 )
#endif

namespace test_str
{
  void print_exc_caught(const csl::common::excbase & e)
  {
    printf("[%s:%d] %s :: %s (%s) caught\n",
        e.file(),
        e.line(),
        e.clazz(),
        e.func(),
        e.reason());
  }

  void empty_checks()
  {
    str s;
    assert( s.empty() == true );
    assert( s.size() == 0 );
  }

  void empty_comparisons()
  {
    str s1,s2;
    assert( s1 == s2 );
    assert( s1 == "" );
    ASSERT_OR_EXC( (s1 == static_cast<const char *>(0)) == true );
    ASSERT_OR_EXC( (s1 == static_cast<const wchar_t *>(0)) == true );
    ASSERT_OR_EXC( (s1 == static_cast<char *>(0)) == true );
    ASSERT_OR_EXC( (s1 == static_cast<wchar_t *>(0)) == true );
  }

  void str_copy()
  {
    str s1(L"Hello world");
    str s2;
    assert( !(s1 == s2) );
    str s3(s1);
    assert( s3 == s1 );
    s2 = s1;
    assert( s2 == s1 );
    assert( s2.size() > 0 && s2.size() == s3.size() && s3.size() == s1.size() );
    assert( s1[3] == s2[3] && s2[4] == s3[4] && s3[5] == s1[5] );
  }

  void str_append()
  {
    str s1(L"Hello ");
    str s2(L"world");
    str s3(L"Hello world");
    str s4(s1+s2);
    assert( s1 == L"Hello ");
    assert( !(s1 == L"He") );
    s1 += s2;
    assert( s1 == s3 );
    assert( s1 == s4 );
  }

  void str_substr()
  {
    str s1(L"Hello world");
    str s2(s1.substr(2,3));
    assert( s2 == L"llo" );
    assert( s1 == L"Hello world");
  }

  void str_trim()
  {
    str s1(L" \t Hello world \t \r\n");
    str s2(s1.trim());
    assert( s2 == L"Hello world");
    assert( s1 == L" \t Hello world \t \r\n");
  }

  void charptr_copy()
  {
    str s1("Hello world");
    str s2(L"Hello world");
    assert( s1 == s2 );
    str s3;
    s3 = "Hello world";
    assert( s1 == s3 );
    assert( s3 == s2 );
    str s4 = s1;
    assert( s1 == s4 );
    assert( s4 == s2 );
  }

  void stdstring_copy()
  {
    std::string x("Hello world");
    str s0,s1(x),s2;
    s2 = x;
    assert( s1 != s0 );
    assert( !(s1 == s0) );
    assert( s1 == "Hello world" );
    assert( s1 == s2 );
  }

  void charptr_append()
  {
    str s1;
    s1 += "Hello";
    assert( s1 == "Hello" );
    assert( (s1 += "") == "Hello" );
    s1 += " world";
    assert( s1 == "Hello world" );
    const char * p = 0;
    ASSERT_OR_EXC( (s1 += p) == "Hello world" );
    s1 += '!';
    ASSERT_OR_EXC( (s1 += '\0') == "Hello world!" );
    assert( s1 == "Hello world!" );
  }

  void stdstring_append()
  {
    str s1;
    std::string h("Hello"), w("world"), e;
    s1 += h;
    assert( s1 == "Hello" );
    s1 += ' ';
    s1 += w;
    assert( s1 == "Hello world");
    s1 += e;
    assert( s1 == "Hello world");
  }

  void comparisons()
  {
    str s1(L"Hello world");
    assert( s1 == "Hello world");
    assert( s1 == L"Hello world");
    assert( s1 !=  "H" );
    assert( s1 !=  "Hello world!" );
    assert( s1 != L"H" );
    assert( s1 != L"Hello world!" );
    assert( s1 != "" );
    assert( s1 != L"" );
    const char * p = 0;
    ASSERT_OR_EXC( (s1.operator==(p)) == false );
    const wchar_t * wp = 0;
    ASSERT_OR_EXC( (s1.operator==(wp)) == false );
  }

  void find()
  {
    str empty, hw(L"Hello world");

    {
      char z          = zero<char>::val_;
      char nz         = 'a';
      const char * zs = "";
      const char * nf = "wl";
      const char * fn = "worl";
      char H          = 'H';
      char d          = 'd';

      ASSERT_OR_EXC( empty.find(z) == str::npos );
      assert( empty.find(nz) == str::npos );
      ASSERT_OR_EXC( empty.find(zs) == str::npos );
      assert( empty.find(nf) == str::npos );
      assert( empty.find(fn) == str::npos );

      ASSERT_OR_EXC( hw.find(z) == str::npos );
      assert( hw.find(nz) == str::npos );
      ASSERT_OR_EXC( hw.find(zs) == str::npos );
      assert( hw.find(nf) == str::npos );
      assert( hw.find(fn) == 6 );
      assert( hw.find(H) == 0 );
      assert( hw.find(d) == 10 );
    }

    {
      wchar_t z          = zero<wchar_t>::val_;
      wchar_t nz         = L'a';
      const wchar_t * zs = L"";
      const wchar_t * nf = L"wl";
      const wchar_t * fn = L"worl";
      wchar_t H          = L'H';
      wchar_t d          = L'd';

      ASSERT_OR_EXC( empty.find(z) == str::npos );
      assert( empty.find(nz) == str::npos );
      ASSERT_OR_EXC( empty.find(zs) == str::npos );
      assert( empty.find(nf) == str::npos );
      assert( empty.find(fn) == str::npos );

      ASSERT_OR_EXC( hw.find(z) == str::npos );
      assert( hw.find(nz) == str::npos );
      ASSERT_OR_EXC( hw.find(zs) == str::npos );
      assert( hw.find(nf) == str::npos );
      assert( hw.find(fn) == 6 );
      assert( hw.find(H) == 0 );
      assert( hw.find(d) == 10 );
    }
  }

  void rfind()
  {
    str empty, hw(L"Hello world");
    wchar_t z  = zero<wchar_t>::val_;
    wchar_t x  = L'x';
    wchar_t e  = L'e';
    wchar_t H  = L'H';
    wchar_t d  = L'd';


    ASSERT_OR_EXC( empty.rfind(z) == str::npos );
    assert( empty.rfind(x) == str::npos );
    assert( empty.rfind(e) == str::npos );

    ASSERT_OR_EXC( hw.rfind(z) == str::npos );
    assert( hw.rfind(x) == str::npos );
    assert( hw.rfind(e) == 1 );
    assert( hw.rfind(d) == 10 );
    assert( hw.rfind(H) == 0 );
  }

  void assign()
  {
    str empty, hw(L"Hello world");
    const wchar_t * np = 0;
    const wchar_t * hl = L"Hello world";
    const wchar_t * end = hl + 11;
    const wchar_t * m0 = hl + 6;
    const wchar_t * m1 = hl + 9;

    str s0;
    ASSERT_OR_EXC( s0.assign(np,np) == empty );
    ASSERT_OR_EXC( s0.assign(hl,np) == empty );
    ASSERT_OR_EXC( s0.assign(np,end) == empty );
    ASSERT_OR_EXC( s0.assign(end,hl) == empty );
    ASSERT_OR_EXC( s0.assign(m1,m0) == empty );

    str s1;
    assert( s1.assign( hl,hl ) == empty );
    assert( s1.assign( hl,end ) == hl );
    assert( s1.assign( hl,end ) == hw );

    str s2;
    assert( s1.assign( m0,m1 ) == L"wor" );
  }

} // end of test_str

using namespace test_str;

int main()
{
  try
  {
    empty_checks();
    empty_comparisons();
    str_copy();
    str_append();
    str_substr();
    str_trim();
    charptr_copy();
    stdstring_copy();
    charptr_append();
    stdstring_append();
    comparisons();
    find();
    rfind();
    assign();
  }
  catch( csl::common::excbase & e )
  {
    print_exc_caught( e );
    throw;
  }
  return 0;
}

// EOF
