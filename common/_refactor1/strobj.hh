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

Credits: some techniques and code pieces are borrowed from Christian
         Stigen Larsen http://csl.sublevel3.org/programming/my_str/
*/

#ifndef _csl_common_str_hh_included_
#define _csl_common_str_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#include "codesloop/common/preallocated_array.hh"
#include "codesloop/common/var.hh"
#include "codesloop/common/binry.hh"
#include "codesloop/common/archiver.hh"
#include <wctype.h>
#ifdef __cplusplus
#include <string>

namespace csl
{
  namespace common
  {
    class ustr;

    class str : public csl::common::var
    {
      CSL_OBJ(csl::common,str);
      public:
        enum {
          buf_items   = 128,
          buf_size    = 128 * sizeof(wchar_t),
          npos        = 0xFFFFFFFF,
          var_type_v  = CSL_TYPE_STR
        };

        typedef const wchar_t * value_t;
        inline value_t value() const { return c_str(); }

        inline virtual int var_type() const { return var_type_v; }        
        str();
        virtual ~str() {}

        /* ------------------------------------------------------------------------ *
        **    str operations
        ** ------------------------------------------------------------------------ */

        str(const str& s);
        str& operator=(const str& s);
        str& operator+=(const str&);

        inline friend str operator+(const str& lhs, const str& rhs)
        {
          return str(lhs) += rhs;
        }

        inline bool operator==(const str& s) const
        {
          int ret = wcscmp( data(), s.data() );
          return (ret == 0);
        }

        size_t find(const str & s) const;
        str substr(const size_t start, const size_t length) const;
        str trim();

        inline bool to_string(str & v) const { v.buf_ = buf_; return true; }
        inline bool from_string(const str & v)   { buf_ = v.buf_; return true; }

        /* ------------------------------------------------------------------------ *
        **    ustr operations
        ** ------------------------------------------------------------------------ */

        explicit str(const ustr & other);
        str & operator=(const ustr & other);
        str& operator+=(const ustr&);
        inline bool operator==(const ustr & s) const
        {
          str rhs(s);
          return (*this == rhs);
        }
        bool to_string(ustr & v) const;
        inline bool from_string(const ustr & v)  { *this = v; return true; }

        /* ------------------------------------------------------------------------ *
        **    char * operations
        ** ------------------------------------------------------------------------ */

        explicit str(const char *);
        str& operator=(const char *);
        inline bool operator==(const char * s) const
        {
          str rhs(s);
          return (*this == rhs);
        }
        bool from_string(const char * v);

        /* ------------------------------------------------------------------------ *
        **    wchar_t * operations
        ** ------------------------------------------------------------------------ */

        str(const wchar_t * wcs);
        str& operator=(const wchar_t * wcs);
        str& operator+=(const wchar_t * str);

        inline friend str operator+(const wchar_t * lhs, const str& rhs)
        {
          return str(lhs) += rhs;
        }

        inline friend str operator+(const str& lhs, const wchar_t * rhs)
        {
          return str(lhs) += rhs;
        }

        inline bool operator==(const wchar_t * s) const
        {
          int ret = wcscmp( data(), s );
          return (ret == 0);
        }

        inline const wchar_t * c_str() const
        {
          return( data() ); // TODO: have a char * version
        }

        str & assign(const wchar_t * start, const wchar_t * end);
        size_t find(const wchar_t * s) const;
        inline const wchar_t * data() const { return buf_.data(); }
        bool from_string(const wchar_t * v);

        /* ------------------------------------------------------------------------ *
        **    std::string operations
        ** ------------------------------------------------------------------------ */

        str& operator=(const std::string & s)
        {
          return operator=(s.c_str());
        }
        bool to_string(std::string & v) const;
        bool from_string(const std::string & v);

        /* ------------------------------------------------------------------------ *
        **    char operations
        ** ------------------------------------------------------------------------ */

        /* ------------------------------------------------------------------------ *
        **    wchar_t operations
        ** ------------------------------------------------------------------------ */

        inline wchar_t operator[](const size_t n) const
        {
          return (data())[n];
        }

        wchar_t at(const size_t n) const;
        size_t find(wchar_t w) const;
        size_t rfind(wchar_t w) const;

        /* ------------------------------------------------------------------------ *
        **    int64, int64_t  operations
        ** ------------------------------------------------------------------------ */
        int64 crc64() const;
        inline bool to_integer(int64 & v) const { return v.from_string(data()); }
        inline bool from_integer(const int64 & v ) { return v.to_string(*this); }
        bool to_integer(int64_t & v) const;
        bool from_integer(int64_t v);

        /* ------------------------------------------------------------------------ *
        **    dbl, double  operations
        ** ------------------------------------------------------------------------ */

        inline bool to_double(dbl & v) const { return v.from_string(data()); }
        inline bool from_double(const dbl & v) { return v.to_string(*this); }
        bool to_double(double & v) const;
        bool from_double(double v);

        /* ------------------------------------------------------------------------ *
        **    binry, binary  operations
        ** ------------------------------------------------------------------------ */

        inline bool to_binary(binry & v) const { return v.from_binary(buf_.data(),nbytes()); }
        bool to_binary(unsigned char * v, size_t & sz) const;
        bool to_binary(void * v, size_t & sz) const;
        inline bool from_binary(const binry & v) { return v.to_string(*this); }
        bool from_binary(const unsigned char * v,size_t sz);
        bool from_binary(const void * v,size_t sz);

        /* ------------------------------------------------------------------------ */

        inline void clear() { reset(); }
        void reset();

        inline size_t size() const
        {
          /* I presume the trailing zero is already there */
          return (empty() ? 0 : (buf_.size()-1));
        }

        inline size_t nbytes() const
        {
          return buf_.nbytes();
        }

        inline size_t nchars() const
        {
          // wcstombs should take care of 'combining characters' too
          return empty() ? 0 : ::wcstombs(NULL, data(), 0);
        }

        inline bool empty() const
        {
          return (buf_.size() <= 1);
        }

        inline const preallocated_array<wchar_t,buf_items> & buffer() const
        {
          return buf_;
        }

        inline preallocated_array<wchar_t,buf_items> & buffer()
        {
          return buf_;
        }

        void ensure_trailing_zero();
        const unsigned char * ucharp_data() const;
        inline size_t var_size() const { return nbytes(); }

        /* ------------------------------------------------------------------------ *
        **    other conversion operations
        ** ------------------------------------------------------------------------ */

        inline bool to_var(var & v) const { return v.from_string(data()); }
        inline bool from_var(const var & v) { return v.to_string(*this); }
        
        virtual inline void serialize(archiver & a) { a.serialize(*this); }

        inline str & operator+=(const var & other)
        {
          str tmp;
          tmp.from_var(other);
          return this->operator+=(tmp);
        }

      private:
        preallocated_array<wchar_t,buf_items> buf_;
    };

    // append template
    template <typename ARG>
    str & operator<<(str & s, const ARG & arg)
    {
      return (s.operator+=(arg));
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_str_hh_included_ */
