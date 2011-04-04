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

#include "codesloop/common/stpodary.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include <assert.h>

using csl::common::stpodary;

namespace test_stpodary
{
  void empty_checks()
  {
    stpodary<char,10> t;
    // should always have some stack based space allocated
    // all other checks are done in dbc statements
    assert( t.data() != 0 );
    assert( t.private_data() != 0 );
  }

  void setter_checks()
  {
    stpodary<float,3> t1(1.1f), t2, t3;
    assert( t1[0] == 1.1f );

    float ft[] = { 1.2f, 2.3f };
    t2.set( ft, 2 );
    assert( t2.size() == 2 );
    assert( t2.nbytes() == 2*sizeof(float) );
    assert( t2[0] == 1.2f );
    assert( t2[1] == 2.3f );

    t2.append( ft, 2 );
    assert( t2[0] == 1.2f );
    assert( t2[1] == 2.3f );
    assert( t2[2] == 1.2f );
    assert( t2[3] == 2.3f );
  }

  void copy_checks()
  {
    double d[] = {1.1,2.2};
    stpodary<double,2> t1(3.3), sta, dyn;
    sta.set( d, 2 );
    dyn.set( d, 2 );
    dyn.append( d, 2 );
    stpodary<double,2> t2(t1),t3(dyn),t4(sta);
    
    assert( t1 == t1 && t1 == t2 );
    assert( t3 == t3 && t3 == dyn );
    assert( t4 == t4 && t4 == sta );
    assert( t3 != sta && t4 != dyn );
  }

  void getter_checks()
  {
    double d[] = {1.1,2.2};
    stpodary<double,2> sta, dyn;
    sta.set( d, 2 );
    dyn.set( d, 2 );
    dyn.append( d, 2 );
    
    double x[4];
    assert( sta.get(x) == true );
    assert( x[0] == 1.1 && x[1] == 2.2 );

    ::memset( x,0,sizeof(x) );
    assert( dyn.get(x) == true );
    assert( x[0] == 1.1 && x[1] == 2.2 && x[2] == 1.1 && x[3] == 2.2 );
  }

  void equality_checks()
  {
    stpodary<double,2> t1(3.3), t2;
    assert( t1 == t1 );
    assert( t1 != t2 );
  }

  void allocate_checks()
  {
    stpodary<double,2> t1;
    double * dp = t1.allocate( 0xFFFFFFFFFFFFFFFFULL );
    assert( dp == NULL );
    dp = t1.allocate( 10 );
    assert( dp != NULL );    
  }
  
  void reset_checks()
  {
    stpodary<double,2> t1(3.3);
    t1.reset();
    assert( t1.is_empty() == true );
    assert( t1.is_static() == true );
    assert( t1.size() == 0 );
  }


} // end of test_stpodary

using namespace test_stpodary;

int main()
{
  empty_checks();
  setter_checks();
  copy_checks();
  getter_checks();
  equality_checks();
  allocate_checks();
  reset_checks();

  return 0;
}

/* EOF */
