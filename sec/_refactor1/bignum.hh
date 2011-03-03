/*
Copyright (c) 2008,2009,2010, CodeSLoop Team

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

#ifndef _csl_sec_bignum_hh_included_
#define _csl_sec_bignum_hh_included_

#include "codesloop/common/archiver.hh"
#include "codesloop/common/obj.hh"
#include "codesloop/common/serializable.hh"
#include "codesloop/common/binry.hh"
#ifdef __cplusplus

namespace csl
{
  namespace sec
  {
    class bignum : public csl::common::obj, public csl::common::serializable
    {
      public:
        bignum() : is_negative_(false) {}
        ~bignum() { reset(); }
        bignum(const bignum & other) { *this = other; }

        bignum & operator=(const bignum & other)
        {
          buf_         = other.buf_;
          is_negative_ = other.is_negative_;
          return *this;
        }

        void reset()
        {
          buf_.reset();
          is_negative_ = false;
        }

        unsigned char * allocate(size_t sz)            { return buf_.allocate(sz);          }
        bool set(const unsigned char * dta, size_t sz) { return buf_.from_binary(dta,sz);   }
        bool get(unsigned char * dta, size_t & sz)     { return buf_.to_binary(dta,sz);     }

        void serialize(common::archiver & a);
        void print() const;

        bool operator==(const bignum & other) const
        {
          if( is_negative_ != other.is_negative_ ) return false;
          return (buf_ == other.buf_);
        }

        bool is_empty() const        { return (buf_.var_size()==0); }
        bool has_data() const        { return (is_empty()==false);  }
        size_t size() const          { return buf_.var_size();      }
        void is_negative(bool yesno) { is_negative_ = (yesno?1:0);  }
        bool is_negative() const     { return (is_negative_==1);    }

        const unsigned char * data() const { return buf_.ucharp_data(); }

      private:
        csl::common::binry  buf_;
        int32_t             is_negative_;

        CSL_OBJ(csl::sec,bignum);
        USE_EXC();
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_sec_bignum_hh_included_ */
