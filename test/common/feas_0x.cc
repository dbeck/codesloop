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
#include <cassert>
#include <algorithm>  // for std::for_each
#include <thread>
#include <vector>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <functional> // for std::function
#include <future>

namespace test_0x
{
  void lambda_fun()
  {
    auto fun = [](int x)->int { return 2*x; };
    int(*xf)(int) = fun;
    fun(1);
    xf(1);
  }

  void range_loop()
  {
    int sum = 0;
    std::vector<int> v{1,2,3,4,5};
    //for(auto x : v) { sum += x; } // not in GCC 4.5
    for( auto it=v.begin(); it!=v.end(); ++it ) { sum += *it; }
  }

  void function_obj()
  {
    std::function< int (int x)> fun;
  }

  void initializers()
  {
    auto v = new std::vector<int>{1,2,3};
    delete v;
  }

  constexpr int cexpr(int a,int b) { return a+b; }

  template<class T> T sum_this(std::initializer_list<T> a)
  {
    T ret = 0;
    std::for_each(a.begin(), a.end(), [&](const T & i){ ret += i; } );
    return ret;
  }

  void rvalues()
  {
    auto && v = std::vector<int>{1,2,3};
    std::vector<int> v2 = v;
    std::vector<int> v3 = v;
  }

  void start_thread()
  {
    std::thread t1(rvalues);
    std::thread t2(rvalues);
    std::thread t3(rvalues);
    t1.join();
    t2.join();
    t3.join();
  }

  void condvar()
  {
    std::condition_variable v;
  }

  void mtx()
  {
    std::mutex m;
    std::unique_lock<std::mutex> l(m);
  }

  void once()
  {
    // from <mutex>
    std::once_flag fl;
  }

  void futprom()
  {
    std::future<int> f;
    std::promise<int> p;
  }

  void binders()
  {
    std::vector<int> x{0,1,2};
    auto y=std::bind(&std::vector<int>::size,x);
    assert( y() == 3 );
  }

  // static void * cptr = std::nullptr; // not in gcc 4.5
}

using namespace test_0x;

int main()
{
  lambda_fun();
  range_loop();
  function_obj();
  initializers();
  if( sum_this({1,2,3}) != 6 ) throw "WTF";
  if( sum_this({1.1,2.2,3.3}) != 6.6 ) throw "WTF";
  rvalues();
  start_thread();
  condvar();
  mtx();
  once();
  futprom();
  binders();
  return 0;
}

// EOF
