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

#ifndef _csl_common_ksmsg_hh_included_
#define _csl_common_ksmsg_hh_included_
#include "codesloop/common/dbc.hh"
#include "codesloop/common/kspin.hh"
#include "codesloop/common/ksbuf.hh"

namespace csl
{
  namespace common
  {
    class ksmsg
    {
    public:
      CSL_CLASS( csl::common::ksmsg );

      struct buf
      {
        uint8_t * buf_;
        size_t    len_;
        inline buf() : buf_(0), len_(0) {}
        inline buf(uint8_t * b,size_t l) : buf_(b), len_(l) {}
        template <typename T> buf(T & t) : buf_(t.buf()), len_(t.len()) {}
      };

      inline ksmsg(const buf & buff, const common::kspin_lock & lck)
        : buf_(&buff), lck_(lck)
      {
      }

      inline const buf & buffer() const      { return *buf_; }
      inline common::kspin_lock & get_lock() { return lck_;  }

      inline ~ksmsg() {}

    private:
      const buf *          buf_;
      common::kspin_lock   lck_;

      ksmsg() = delete;
    };
  }
}

#endif /*_csl_common_ksmsg_hh_included_*/
