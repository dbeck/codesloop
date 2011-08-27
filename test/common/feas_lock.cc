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
#include "codesloop/common/lock.hh"
#include <pthread.h>
#include <mutex>

//using namespace csl::common;

namespace test_lock
{
  static inline unsigned int xchg32(void * ptr, unsigned int x)
  {
    __asm__ __volatile__ (
        "xchgl %0, %1"
        :"=r" (static_cast<unsigned int>(x))
        :"m" (*static_cast<volatile unsigned *>(ptr)), "0" (x)
        :"memory" );
    return x;
  }

#define CPURELAX() asm volatile("pause\n": : :"memory")
#define BARRIER() asm volatile("": : :"memory")
#define CMPXCHG(P, O, N) __sync_val_compare_and_swap((P),(O),(N))
#define AINC(P) __sync_add_and_fetch((P),1)
#define XADD(P, V) __sync_fetch_and_add((P),(V))

  static const unsigned int BUSY = 1;
  typedef unsigned int spin;

  static inline void spin_lock(spin * lock)
  {
    while(1)
    {
      if(!xchg32(lock,BUSY)) return;
      while(*lock) CPURELAX();
    }
  }

  static inline void spin_unlock(spin * lock)
  {
    BARRIER();
    *lock = 0;
  }

  typedef union rwticket rwticket;

  union rwticket
  {
    unsigned int u;
    unsigned short us;
    __extension__ struct
    {
      unsigned char write;
      unsigned char read;
      unsigned char users;
    } s;
  };

  static void rwt_lock(rwticket * l)
  {
    unsigned int me = XADD(&l->u, (1<<16));
    unsigned char val = static_cast<unsigned char>(me >> 16);
    while( val != l->s.write ) CPURELAX();
  }

  static void rwt_unlock(rwticket * l)
  {
    rwticket t = *l;
    BARRIER();
    t.s.write++;
    t.s.read++;
    l->us = t.us;
  }

  static void rwt_rlock(rwticket * l)
  {
    unsigned int me = XADD(&l->u, (1<<16));
    unsigned char val = static_cast<unsigned char>(me >> 16);
    while( val != l->s.read ) CPURELAX();
    l->s.read++;
  }

  static void rwt_runlock(rwticket * l)
  {
    AINC(&l->s.write);
  }

  static rwticket           rwt;
  static rwticket           rwt2;
  static spin               mspin = 0;
  static pthread_mutex_t    mutex;
  static pthread_rwlock_t   rwlock;
  static pthread_spinlock_t spinlock;
  static std::mutex         stdmutex;

  void rwt_test()
  {
    rwt_lock(&rwt);
    rwt_unlock(&rwt);
  }

  void rwt_rtest()
  {
    rwt_rlock(&rwt2);
    rwt_runlock(&rwt2);
  }

  void spin_test()
  {
    spin_lock(&mspin);
    spin_unlock(&mspin);
  }

  void pthread_mutex_test()
  {
    pthread_mutex_lock( &mutex );
    pthread_mutex_unlock( &mutex );
  }

  void pthread_rwlock_test()
  {
    pthread_rwlock_wrlock( &rwlock );
    pthread_rwlock_unlock( &rwlock );
  }

  void pthread_spinlock_test()
  {
    pthread_spin_lock( &spinlock );
    pthread_spin_unlock( &spinlock );
  }

  void std_mutex_test()
  {
    stdmutex.lock();
    stdmutex.unlock();
  }
}

using namespace test_lock;

int main()
{
  rwt.u = 0;
  pthread_mutex_init( &mutex, NULL );
  pthread_spin_init( &spinlock, 0 );
  pthread_rwlock_init( &rwlock, 0 );

  csl_common_print_results( "pthread mutex     ", csl_common_test_timer_v0(pthread_mutex_test),"" );
  csl_common_print_results( "pthread rwlock    ", csl_common_test_timer_v0(pthread_rwlock_test),"" );
  csl_common_print_results( "pthread spinlock  ", csl_common_test_timer_v0(pthread_spinlock_test),"" );
  csl_common_print_results( "spin              ", csl_common_test_timer_v0(spin_test),"" );
  csl_common_print_results( "rwt               ", csl_common_test_timer_v0(rwt_test),"" );
  csl_common_print_results( "rwt-rd            ", csl_common_test_timer_v0(rwt_rtest),"" );
  csl_common_print_results( "stdmutex          ", csl_common_test_timer_v0(std_mutex_test),"" );

  pthread_rwlock_destroy( &rwlock );
  pthread_spin_destroy( &spinlock );
  pthread_mutex_destroy( &mutex );
  return 0;
}

// EOF
