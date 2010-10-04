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

#ifndef _csl_common_xdrbuf_hh_included_
#define _csl_common_xdrbuf_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/pbuf.hh"
#ifdef __cplusplus
#include <utility>

namespace csl
{
  namespace common
  {
    class str;
    class ustr;
    class var;
    class exc;

    class xdrbuf_old0 : public obj
    {
      CSL_OBJ(csl::common,xdrbuf_old0);

      public:
        inline xdrbuf_old0(pbuf & b)
        : use_exc_(true), b_(&b), it_(b.begin()), pos_(0) { }

        inline xdrbuf_old0(const xdrbuf_old0 & x)
        : use_exc_(x.use_exc_), b_(x.b_), it_(x.it_), pos_(x.pos_) { }

        ~xdrbuf_old0() {}

        inline xdrbuf_old0 & operator=(const xdrbuf_old0 & x)
        {
          b_       = x.b_;
          it_      = x.it_;
          pos_     = x.pos_;
          use_exc_ = x.use_exc_;
          return *this;
        }

        typedef std::pair<const unsigned char *,uint64_t> bindata_t;

        xdrbuf_old0 & operator<<(int32_t val);
        xdrbuf_old0 & operator<<(uint32_t val);
        xdrbuf_old0 & operator<<(int64_t val);
        xdrbuf_old0 & operator<<(uint64_t val);
        xdrbuf_old0 & operator<<(const char * val);
//        xdrbuf_old0 & operator<<(const common::serializable & val);
//        xdrbuf_old0 & operator<<(const common::var & val);
        xdrbuf_old0 & operator<<(const common::str & val);
        xdrbuf_old0 & operator<<(const common::ustr & val);
        xdrbuf_old0 & operator<<(const bindata_t & val);
        xdrbuf_old0 & operator<<(const pbuf & val);
        
        xdrbuf_old0 & operator>>(int32_t & val);
        xdrbuf_old0 & operator>>(uint32_t & val);
        xdrbuf_old0 & operator>>(int64_t & val);
        xdrbuf_old0 & operator>>(uint64_t & val);
//        xdrbuf_old0 & operator>>(common::serializable & val);
//        xdrbuf_old0 & operator>>(common::var & val);
        xdrbuf_old0 & operator>>(common::str & val);
        xdrbuf_old0 & operator>>(common::ustr & val);
        xdrbuf_old0 & operator>>(pbuf & val);

        uint64_t get_data( unsigned char * where,
                           uint64_t size );

        bool get_data( unsigned char * where,
                       uint64_t & size,
                       uint64_t max_size );

        template <typename T>
        bool get_data( T & t,
                       uint64_t & size,
                       uint64_t max_size )
        {
          pbuf::iterator oldit = it_;
          uint64_t oldpos  = pos_;
          uint64_t sz;
          (*this) >> sz;
          it_  = oldit;
          pos_ = oldpos;
          if( sz <= 0 || sz > max_size ) return false;
          return get_data( reinterpret_cast<unsigned char *>(t.allocate(sz)), size, max_size );
        }

        bool forward(uint64_t n);

        inline void use_exc(bool yesno) { use_exc_ = yesno; }
        inline bool use_exc() const { return use_exc_; }

        inline bool operator==(const xdrbuf_old0 & other) const
        {
          return (use_exc_ == other.use_exc_ && b_ == other.b_);
        }

        void rewind();
        uint64_t position();

      private:
        bool use_exc_;
        pbuf * b_;
        pbuf::iterator it_;
        uint64_t pos_;

        xdrbuf_old0() : use_exc_(true), b_(0), it_(0,0), pos_(0) {}
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_xdrbuf_hh_included_ */
