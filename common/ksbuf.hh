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
        inline uint8_t * buf()       { return buf_; }
        inline size_t len()          { return buf_size_; }
        inline uint32_t id()         { return id_; }
        inline kspin_lock get_lock() { return kspin_lock(*spin_,id_); }
        inline void clear()          { spin_ = 0; id_ = -1; buf_ = 0; }
        inline bool is_valid() const { return (spin_ != 0 && buf_ != 0); }
      };

      static const uint32_t buf_size_=BUF_SIZE;
      static const uint32_t buf_count_=BUF_COUNT;
      static const uint32_t total_buffer_len_=(buf_size_*buf_count_);
      static const uint32_t buf_mask_=(buf_size_|(buf_size_-1));

      inline ksbuf() : act_pos_(0) {}

      inline void get(result & res)
      {
        // ??? XXX TODO : multithreaded get() ?

        uint32_t pos = (++act_pos_)%buf_count_;

        while( locks_[pos].is_locked() )
        {
          pos = (++act_pos_)%buf_count_;
        }

        res.buf_  = buffer_+(buf_size_*pos);
        res.id_   = locks_[pos].gtinc();
        res.spin_ = locks_+pos;
      }

      inline result get()
      {
        result ret;
        get(ret);
        return ret;
      }

    private:
      uint8_t                    buffer_[total_buffer_len_];
      kspin                      locks_[buf_count_];
      std::atomic_uint_fast32_t  act_pos_;
    };
  }
}

#endif /*_csl_common_ksbuf_hh_included_*/
