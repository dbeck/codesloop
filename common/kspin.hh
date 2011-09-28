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

#ifndef _csl_common_kspin_hh_included_
#define _csl_common_kspin_hh_included_
#include "codesloop/common/common.h"
#include <atomic>

namespace csl
{
  namespace common
  {
    class kspin
    {
    public:
      CSL_CLASS( csl::common::kspin );

      static const uint32_t init_ = 0xffffffff;

      inline kspin(uint32_t init=init_) : spin_(init_) {}

      // old => new
      // locks the kspin to new id
      inline void xlock(uint32_t old_id, uint32_t new_id)
      {
        uint32_t old_=old_id;
        while( !spin_.compare_exchange_strong(old_,new_id) )
        {
          old_=old_id;
        }
      }

      // id => 0 except id mismatch
      inline bool lock(uint32_t id)
      {
        uint32_t id_=id;
        uint32_t busy=0;
        while( !spin_.compare_exchange_strong(id_,busy) )
        {
          if( id_!=id ) return false;
          id_=id;
        }
        return true;
      }

      // 0 => id
      inline void unlock(uint32_t id) { spin_.store(id); }

      inline uint32_t load() const { return spin_.load(); }

    private:
      std::atomic_uint_fast32_t spin_;
    };
  }
}

#endif /* _csl_common_kspin_hh_included_ */
