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

#ifndef _csl_common_ksbuf_hh_included_
#define _csl_common_ksbuf_hh_included_
#include "codesloop/common/kspin.hh"

namespace csl
{
  namespace common
  {
    template <uint32_t BUF_COUNT=512,uint32_t BUF_SIZE=65536> // 32 MB
    class ksbuf
    {
    public:
      CSL_CLASS( csl::common::ksbuf );

      struct result
      {
        kspin *    spin_;
        uint32_t   id_;
        uint8_t *  buf_;
      };

      static const uint32_t buf_size_=BUF_SIZE;
      static const uint32_t buf_count_=BUF_COUNT;
      static const uint32_t total_buffer_len_=(buf_size_*buf_count_);
      static const uint32_t buf_mask_=(buf_size_|(buf_size_-1));

      inline ksbuf() : act_id_(0) {}

      inline void get(result & res)
      {
        ++act_id_;
        if( act_id_ == 0 ) ++act_id_;
        uint32_t act_pos = act_id_%buf_count_;
        // try not to get blocked (no guarantee though)
        while( locks_[act_pos].load()==0 )
        {
          ++act_id_;
          if( act_id_ == 0 ) ++act_id_;
          act_pos = act_id_%buf_count_;
        }
        res.buf_   = buffer_+(buf_size_*act_pos);
        res.id_    = act_id_;
        res.spin_  = locks_+act_pos;
        uint32_t last_id = (act_id_ < (buf_count_+1) ? kspin::init_ : (act_id_-buf_count_));
        // invalidate buffer (may still block)
        locks_[act_pos].xlock(last_id,act_id_);
      }

    private:
      uint8_t   buffer_[total_buffer_len_];
      kspin     locks_[buf_count_];
      uint32_t  act_id_;
    };
  }
}

#endif /*_csl_common_ksbuf_hh_included_*/
