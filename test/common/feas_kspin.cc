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
#include "codesloop/common/test_timer.h"
#include "codesloop/common/kspin.hh"
#include <assert.h>
#include <thread>
#include <vector>
#include <set>

using namespace csl::common;

namespace test_kspin
{
  void baseline()
  {
    kspin ks;
  }

  static kspin global_ks;

  void gtinc()
  {
    global_ks.gtinc();
  }

  void lock()
  {
    kspin ks;
    ks.lock(kspin::init_);
  }

  void unlock()
  {
    global_ks.unlock(kspin::init_);
  }

  void lock_unlock()
  {
    global_ks.lock(kspin::init_);
    global_ks.unlock(kspin::init_);
  }

  void atomic_inc()
  {
    static std::atomic_uint_fast32_t l;
    ++l;
  }

  void validate()
  {
    kspin ks;
    assert(ks.load()==kspin::init_);
    ks.gtinc();
    {
      assert(ks.lock(kspin::init_+1) == true);
      assert(ks.load()==0);
      ks.unlock(kspin::init_+1);
      assert(ks.load()==kspin::init_+1);
    }

    assert(ks.lock(kspin::init_+2) == false);
    assert(ks.load()==kspin::init_+1);

    {
      assert(ks.lock(kspin::init_+1) == true);
      ks.unlock(kspin::init_+1);
      assert(ks.load()==kspin::init_+1);
    }
  }

  namespace thr
  {
    typedef std::set<uint32_t> intset;

    static const char * class_name() { return "thr::tester"; }
    CSL_DECLARE_EXCEPTION(gtinc_equals_old_value);
    CSL_DECLARE_EXCEPTION(duplicate_value);

    void test_entry(kspin * s, intset * v)
    {
      for(int i=0;i<1000;++i)
      {
        uint32_t old = s->load();
        uint32_t id = s->gtinc();
        // printf("%d->%d ",old,id);
        if( old == id )      { CSL_THROW(gtinc_equals_old_value); }
        if( id > 0xffffff )  { printf("id: %d\n", id); }
        if( s->lock(id) )
        {
          if( v->find(id) != v->end() )
          {
            printf("duplicate value: %d\n",id);
            CSL_THROW(duplicate_value);
            break;
          }
          v->insert(id);
          s->unlock(id);
        }
      }
    }

    void tester(kspin & s, int index, intset * is)
    {
      if(index > 4) return;
      for( int x=0;x<3;++x ) { tester(s,index+1,is); }
      std::thread t1(std::bind(test_entry,&s,is));
      std::thread t2(std::bind(test_entry,&s,is));
      std::thread t3(std::bind(test_entry,&s,is));
      std::thread t4(std::bind(test_entry,&s,is));
      std::thread t5(std::bind(test_entry,&s,is));
      t1.join(); t2.join(); t3.join(); t4.join(); t5.join();
    }
  }
}

using namespace test_kspin;

int main()
{
  thr::intset is;
  kspin ks;
  thr::tester(ks,0,&is);

  csl_common_print_results( "baseline                 ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "gtinc                    ", csl_common_test_timer_v0(gtinc),"" );
  global_ks.unlock(kspin::init_);
  csl_common_print_results( "lock                     ", csl_common_test_timer_v0(lock),"" );
  csl_common_print_results( "unlock                   ", csl_common_test_timer_v0(unlock),"" );
  csl_common_print_results( "lock-unlock              ", csl_common_test_timer_v0(lock_unlock),"" );
  csl_common_print_results( "atomic_inc               ", csl_common_test_timer_v0(atomic_inc),"" );
  return 0;
}
