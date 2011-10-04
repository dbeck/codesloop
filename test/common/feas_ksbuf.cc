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

#include "codesloop/common/kspin.hh"
#include "codesloop/common/excbase.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/ksbuf.hh"
#include <memory>
#include <assert.h>

using namespace csl::common;

namespace test_ksbuf
{
  struct alloc_result
  {
    kspin *    spin_;
    uint32_t   id_;
    uint8_t *  buf_;
  };

  class ksbuf2
  {
  public:
    static const uint32_t max_buf_=65536;
    static const uint32_t buf_count_=512;
    static const uint32_t total_buffer_len_=(max_buf_*buf_count_); //32MB

    inline ksbuf2() : act_id_(0) {}

    inline void get(alloc_result & res)
    {
      ++act_id_;
      if( act_id_ == 0 ) ++act_id_;
      uint32_t act_pos = act_id_%buf_count_;
      // try not to get blocked
      while( locks_[act_pos].load()==0 )
      {
        ++act_id_;
        if( act_id_ == 0 ) ++act_id_;
        act_pos = act_id_%buf_count_;
      }
      res.buf_   = buffer_+(max_buf_+act_pos);
      res.id_    = act_id_;
      res.spin_  = locks_+act_pos;
      // invalidate buffer (may still block) but don't care
      uint32_t last_id = (act_id_ < (buf_count_+1) ? kspin::init_ : (act_id_-buf_count_));
      locks_[act_pos].xlock(last_id,act_id_);
    }

  private:
    uint8_t   buffer_[total_buffer_len_];
    kspin     locks_[buf_count_];
    uint32_t  act_id_;
  };

  void ksbuf2_baseline()
  {
    ksbuf2 * b = new ksbuf2;
    delete b;
  }

  ksbuf2 * ksbuf2_ = 0;

  void ksbuf2_get()
  {
    alloc_result res;
    ksbuf2_->get(res);
  }

  void ksbuf_baseline()
  {
    ksbuf<> * b = new ksbuf<>();
    delete b;
  }

  ksbuf<> * ksbuf_ = 0;

  void ksbuf_get()
  {
    ksbuf<>::result res;
    ksbuf_->get(res);
  }

}

using namespace test_ksbuf;

int main()
{
  try
  {
    ksbuf2_ = new ksbuf2;
    ksbuf_ = new ksbuf<>();
    csl_common_print_results( "ksbuf_baseline                ", csl_common_test_timer_v0(ksbuf_baseline),"" );
    csl_common_print_results( "ksbuf2_baseline                ", csl_common_test_timer_v0(ksbuf2_baseline),"" );
    csl_common_print_results( "ksbuf_get                     ", csl_common_test_timer_v0(ksbuf_get),"" );
    csl_common_print_results( "ksbuf2_get                     ", csl_common_test_timer_v0(ksbuf2_get),"" );
  }
  catch(const excbase & e)
  {
    e.print();
  }
  delete ksbuf2_;
  return 0;
}

// EOF

