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

#ifdef DEBUG
#define DEBUG_ENABLE_INDENT
#define DEBUG_VERBOSE
#endif /* DEBUG */

#include "codesloop/common/simpstr.hh"
#include "codesloop/common/excbase.hh"
#include <string>
#include <assert.h>

using csl::common::simpstr;
using csl::common::excbase;

namespace test_simpstr
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
    simpstr s;
    assert( s.empty() == true );
    assert( s.size() == 0 );
  }
  
  void empty_comparisons()
  {
    simpstr s1,s2;
    assert( s1 == s2 );
    assert( s1 == "" );
    
    {
      bool caught = false;
      try
      {
        assert( s1 == static_cast<char *>(0) );
      }
      catch( csl::common::excbase & e )
      {
        caught = true;
        print_exc_caught( e );
      }
      assert( caught == true );
    }

    {
      bool caught = false;
      try
      {
        assert( s1 == static_cast<wchar_t *>(0) );
      }
      catch( csl::common::excbase & e )
      {
        caught = true;
        print_exc_caught( e );
      }
      assert( caught == true );
    }
  }
  
  void str_copy()
  {
    simpstr s1(L"Hello world");
    simpstr s2;
    assert( !(s1 == s2) );
    simpstr s3(s1);
    assert( s3 == s1 );
    s2 = s1;
    assert( s2 == s1 );
    assert( s2.size() > 0 && s2.size() == s3.size() && s3.size() == s1.size() );
    assert( s1[3] == s2[3] && s2[4] == s3[4] && s3[5] == s1[5] );
  }
  
  void str_append()
  {
    simpstr s1(L"Hello ");
    simpstr s2(L"world");
    simpstr s3(L"Hello world");
    simpstr s4(s1+s2);
    assert( s1 == L"Hello ");
    assert( !(s1 == L"He") );
    s1 += s2;
    assert( s1 == s3 );
    assert( s1 == s4 );
  }
  
  void str_substr()
  {
    simpstr s1(L"Hello world");
    simpstr s2(s1.substr(2,3));
    assert( s2 == L"llo" );
    assert( s1 == L"Hello world");
  }
  
  void str_trim()
  {
    simpstr s1(L" \t Hello world \t \r\n");
    simpstr s2(s1.trim());
    assert( s2 == L"Hello world");
    assert( s1 == L" \t Hello world \t \r\n");
  }
  
  void charptr_copy()
  {
    simpstr s1("Hello world");
    simpstr s2(L"Hello world");
    assert( s1 == s2 );
    simpstr s3;
    s3 = "Hello world";
    assert( s1 == s3 );
    assert( s3 == s2 );
    simpstr s4 = s1;
    assert( s1 == s4 );
    assert( s4 == s2 );
  }
  
  void stdstring_copy()
  {
    std::string x("Hello world");
    simpstr s0,s1(x),s2;
    s2 = x;
    assert( s1 != s0 );
    assert( !(s1 == s0) );
    assert( s1 == "Hello world" );
    assert( s1 == s2 );
  }
  
  void charptr_append()
  {
    simpstr s1;
    s1 += "Hello";
    assert( s1 == "Hello" );
    s1 += "";
    assert( s1 == "Hello" );
    s1 += " world";
    assert( s1 == "Hello world" );
    bool caught = false;
    try
    {
      char * p = 0;
      s1 += p;
    }
    catch( csl::common::excbase & e )
    {
      caught = true;
      print_exc_caught( e );
    }
    assert( caught == true );
    s1 += '!';
    assert( s1 == "Hello world!" );
    s1 += '\0';
    assert( s1 == "Hello world!" );
  }

  void stdstring_append()
  {
    simpstr s1;
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
    simpstr s1(L"Hello world");
    assert( s1 == "Hello world");
    assert( s1 == L"Hello world");
    assert( s1 !=  "H" );
    assert( s1 !=  "Hello world!" );
    assert( s1 != L"H" );
    assert( s1 != L"Hello world!" );
    assert( s1 != "" );
    assert( s1 != L"" );
    
    {
      bool caught = false;
      try
      {
        char * p = 0;
        s1.operator==(p);
      }
      catch( csl::common::excbase & e )
      {
        caught = true;
        print_exc_caught( e );
      }
      assert( caught == true );
    }

    {
      bool caught = false;
      try
      {
        wchar_t * p = 0;
        s1.operator==(p);
      }
      catch( csl::common::excbase & e )
      {
        caught = true;
        print_exc_caught( e );
      }
      assert( caught == true );
    }    
  }
  
  void find()
  {
  }
  
  void rfind()
  {
  }
  
  void assign()
  {
  }
  
  
    
  
} // end of test_simpstr

using namespace test_simpstr;

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
