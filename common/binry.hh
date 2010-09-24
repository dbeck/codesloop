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
THEORY OF LIABILITY, WHETHER IN CONTRACT, objICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _csl_common_binry_hh_included_
#define _csl_common_binry_hh_included_

#include "codesloop/common/var.hh"
#include "codesloop/common/dbl.hh"
#include "codesloop/common/preallocated_array.hh"
#include "codesloop/common/arch.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class str;
    class ustr;
    class xdrbuf;

    class binry : public csl::common::var
    {
      public:
        enum { buf_size = 1024, var_type_v = CSL_TYPE_BIN };
        typedef preallocated_array<uint8_t,buf_size> buf_t;
        typedef const buf_t & value_t;

      private:
        buf_t value_;

      public:
        binry();

        inline binry & operator=(const binry & other)
        {
          value_ = other.value_;
          return *this;
        }

        binry(const unsigned char * ptr,size_t sz);
        virtual inline ~binry() {}
        inline value_t value() const { return value_; }
        inline int var_type() const { return var_type_v; }
        inline void reset() { value_.reset(); }

        inline const unsigned char * ucharp_data() const { return value_.data(); }
        inline size_t var_size() const { return value_.size(); }

        inline bool to_integer(int64 & v) const { return v.from_binary(value_.data(),value_.size()); }
        bool to_integer(int64_t & v) const;
        inline bool from_integer(const int64 & v) { return v.to_binary(*this); }
        bool from_integer(int64_t v);

        inline bool to_double(dbl & v) const { return v.from_binary(value_.data(),value_.size()); }
        bool to_double(double & v) const;
        inline bool from_double(const dbl & v)  { return v.to_binary(*this); }
        bool from_double(double v);

        bool to_string(str & v) const;
        bool to_string(ustr & v) const;
        bool to_string(std::string & v) const;
        bool from_string(const str & v);
        bool from_string(const ustr & v);
        bool from_string(const std::string & v);
        bool from_string(const char * v);
        bool from_string(const wchar_t * v);
        
        inline bool to_binary(binry & v) const { return v.from_binary(value_.data(),value_.size()); }
        bool to_binary(unsigned char * v, size_t & sz) const;
        bool to_binary(void * v, size_t & sz) const;
        bool from_binary(const binry & v);
        bool from_binary(const unsigned char * v,size_t sz);
        bool from_binary(const void * v,size_t sz);

        bool to_xdr(xdrbuf & b) const;
        bool from_xdr(xdrbuf & v);
        
        inline bool to_var(var & v) const { return v.from_binary(value_.data(),value_.size()); }
        bool from_var(const var & v);

        virtual inline void serialize(arch & buf) { buf.serialize(*this); }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_binry_hh_included_ */
