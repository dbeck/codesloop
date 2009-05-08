/*
Copyright (c) 2008,2009, David Beck

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

/**
   @file t__peer.cc
   @brief Tests to check and measure various sched::peer features
 */

#include "csl_slt3.hh"
#include "test_timer.h"
#include "peer.hh"
#include "exc.hh"
#include "common.h"
#include "mpool.hh"
#include <assert.h>
#include <stdlib.h>

using namespace csl::slt3;
using namespace csl::sched;
using namespace csl::sec;

/** @brief contains tests related to sched::peer */
namespace test_peer {

  /** @todo document me */
  void baseline()
  {
    peer p;
  }

  /** @todo document me */
  void usage1()
  {
    peer p;
    bool caught = false;
    try
    {
      conn & db(p.db());
      tran t(db);
      assert( p.create(t) == false );
    }
    catch( csl::sched::exc e )
    {
      caught = true;
    }
    catch( csl::slt3::exc e )
    {
      caught = true;
    }

    assert( caught == true );
  }

  /** @todo document me */
  void usage2()
  {
    peer p;
    conn & db(p.db());
    tran t(db);
    p.id_ = 199999999;
    assert( p.find_by_id(t) == false );

    p.common_name_ = "Whatever I put here";
    assert( p.find_by(t,1) == false );
  }

  /** @todo document me */
  void usage3()
  {
    peer p;
    conn & db(p.db());
    tran t(db);
    ecdh_key pubk;
    pubk.algname("prime192v3");
    bignum   privk;
    assert( pubk.gen_keypair(privk) == true );
    p.public_key(pubk);
    p.private_key(privk);
    p.common_name_ = "Hello";
    p.create(t);

    peer p2;
    p2.common_name_ = "Hello";
    assert( p2.find_by(t,1) == true );
    assert( p2.public_key().has_data() == true );
    assert( p2.private_key().is_empty() == false );

    peer p3;
    p3.id_ = p2.id_.get();
    assert( p3.find_by_id(t) == true );
    assert( p3.public_key().has_data() == true );
    assert( p3.private_key().is_empty() == false );

    assert( p.remove(t) == true );
  }

  /** @todo document me */
  void usage3_nodel()
  {
    peer p;
    conn & db(p.db());
    tran t(db);
    ecdh_key pubk;
    pubk.algname("prime192v3");
    bignum   privk;
    assert( pubk.gen_keypair(privk) == true );
    p.public_key(pubk);
    p.private_key(privk);
    p.common_name_ = "Hello";
    p.create(t);

    peer p2;
    p2.common_name_ = "Hello";
    assert( p2.find_by(t,1) == true );
    assert( p2.public_key().has_data() == true );
    assert( p2.private_key().is_empty() == false );

    peer p3;
    p3.id_ = p2.id_;
    assert( p3.find_by_id(t) == true );
    assert( p3.public_key().has_data() == true );
    assert( p3.private_key().is_empty() == false );
  }

  /** @todo document me */
  void usage4()
  {
    peer p2;
    conn & db(p2.db());
    tran t(db);
    p2.common_name_ = "Hello";
    assert( p2.find_by(t,1) == true );
    assert( p2.public_key().has_data() == true );
    assert( p2.private_key().is_empty() == false );
  }

  /** @todo document me */
  void usage5()
  {
    peer p2;
    conn & db(p2.db());
    tran t(db);

    p2.common_name_ = "Hello";
    assert( p2.find_by(t,1) == true );

    peer p3;
    p3.public_key(p2.public_key());

    assert( p3.find_by(t,2) == true );
    assert( p3.public_key().is_empty() == false );
    assert( p3.private_key().has_data() == true );
    assert( p3.public_key().algname() == "prime192v3" );
    assert( std::string(p3.common_name_.c_str()) == "Hello" );
    assert( p3.id_.get() == p2.id_.get() );
  }

} // end of test_peer

using namespace test_peer;

int main()
{
  {
    peer p;
    conn & db(p.db());
    tran t(db);
    p.init(t);
  }

  csl_common_print_results( "baseline           ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "usage1             ", csl_common_test_timer_v0(usage1),"" );
  csl_common_print_results( "usage2             ", csl_common_test_timer_v0(usage2),"" );
  csl_common_print_results( "usage3             ", csl_common_test_timer_v0(usage3),"" );
  usage3_nodel();
  csl_common_print_results( "usage4             ", csl_common_test_timer_v0(usage4),"" );
  csl_common_print_results( "usage5             ", csl_common_test_timer_v0(usage5),"" );

  {
    peer p;
    conn & db(p.db());
    tran t(db);
    synqry q(t);
    q.execute("DELETE FROM peers;");
  }

  return 0;
}

/* EOF */
