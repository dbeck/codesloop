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

#include "codesloop/common/excbase.hh"
#include <assert.h>
#include <string.h>

using csl::common::excbase;

namespace test_exc
{
  class test_class
  {
    public:
      CSL_CLASS( test_class );
      CSL_DECLARE_EXCEPTION( test_exc );
      
      void faulty_fun()
      {
        CSL_THROW( test_exc );
      }
  
  };
  
  void print_exc_caught(const excbase & e)
  {
    printf("[%s:%d] %s :: %s (%s) caught\n",
        e.file(),
        e.line(),
        e.clazz(),
        e.func(),
        e.reason()); 
  }  

  void catch_base_ref()
  {
    bool caught = false;
    test_class tc;
    
    try { tc.faulty_fun(); }
    catch( excbase & e )
    {
      assert( strstr( e.file(),"func_exc.cc" ) != NULL );
      assert( strstr( e.func(),"faulty_fun" ) != NULL );
      assert( strstr( e.clazz(),"test_class" ) != NULL );
      assert( strstr( e.reason(),"test_exc" ) != NULL );
      assert( e.line() > 0 );      
      // print_exc_caught( e );
      caught = true;
    }
    assert( caught == true );      
  }

  void catch_base_copy()
  {
    bool caught = false;
    test_class tc;
    
    try { tc.faulty_fun(); }
    catch( excbase e )
    {
      assert( strstr( e.file(),"func_exc.cc" ) != NULL );
      assert( strstr( e.func(),"faulty_fun" ) != NULL );
      assert( strstr( e.clazz(),"test_class" ) != NULL );
      assert( strstr( e.reason(),"test_exc" ) != NULL );
      assert( e.line() > 0 );      
      // print_exc_caught( e );
      caught = true;
    }
    assert( caught == true );      
  }

  void catch_desc_ref()
  {
    bool caught = false;
    test_class tc;
    
    try { tc.faulty_fun(); }
    catch( test_class::test_exc & e )
    {
      assert( strstr( e.file(),"func_exc.cc" ) != NULL );
      assert( strstr( e.func(),"faulty_fun" ) != NULL );
      assert( strstr( e.clazz(),"test_class" ) != NULL );
      assert( strstr( e.reason(),"test_exc" ) != NULL );
      assert( e.line() > 0 );      
      // print_exc_caught( e );
      caught = true;
    }
    assert( caught == true );      
  }
      
} // end of test_exc

using namespace test_exc;

int main()
{
  catch_base_ref();
  catch_base_copy();
  catch_desc_ref();
  return 0;
}

// EOF
