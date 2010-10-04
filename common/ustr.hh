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

Credits: some techniques and code pieces are borrowed from Christian
         Stigen Larsen http://csl.sublevel3.org/programming/my_str/
*/

#ifndef _csl_common_ustr_hh_included_
#define _csl_common_ustr_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#include "codesloop/common/preallocated_array.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/var.hh"
#ifdef __cplusplus
#include <string>

namespace csl
{
  namespace common
  {
    class ustr : public csl::common::var
    {
      CSL_OBJ(csl::common,ustr);
      public:
        enum {
          buf_size = 128,
          npos = 0xFFFFFFFF,
          var_type_v = CSL_TYPE_USTR
        };

        typedef const char * value_t;
        inline value_t value() const { return c_str(); }
        inline virtual int var_type() const { return var_type_v; }

        inline ustr() : csl::common::var()
        {
          buf_.set("\0",1);
        }

        virtual ~ustr() {}

        /* ------------------------------------------------------------------------ *
        **    str operations
        ** ------------------------------------------------------------------------ */

        explicit ustr(const str & other);
        ustr & operator=(const str & other);
        ustr& operator+=(const str&);
        inline bool operator==(const str& s) const
        {
          ustr rhs(s);
          return (*this == rhs);
        }
        inline bool to_string(str & v) const { return v.from_string(data()); }
        inline bool from_string(const str & v) { *this = v; return true; }

        /* ------------------------------------------------------------------------ *
        **    ustr operations
        ** ------------------------------------------------------------------------ */
        
        inline ustr(const ustr& s) : csl::common::var()
        {
          buf_.set("\0",1);
          buf_ = s.buf_;
        }

        inline ustr& operator=(const ustr& s)
        {
          buf_ = s.buffer();
          return *this;
        }

        ustr& operator+=(const ustr&);

        inline friend ustr operator+(const ustr& lhs, const ustr& rhs)
        {
          return ustr(lhs) += rhs;
        }

        inline bool operator==(const ustr& s) const
        {
          return (::strncmp( data(), s.data(), static_cast<size_t>(nbytes()) ) == 0);
        }

        size_t find(const ustr & s) const;
        ustr substr(const size_t start, const size_t length) const;
        inline ustr trim()
        {
            size_t start = npos, length = 0;

            for ( size_t pos = 0; pos < size() ; pos ++ )
            {
                if ( start == npos && !isspace( (*this)[pos] )  )
                    start = pos;
                else if ( start != npos && !isspace( (*this)[pos] ) )
                    length = pos - start + 1;
            }
            return substr(start,length);
        }

        inline bool to_string(ustr & v) const { v.buf_ = buf_; return true; }
        inline bool from_string(const ustr & v) { buf_ = v.buf_; return true; }

        /* ------------------------------------------------------------------------ *
        **    char * operations
        ** ------------------------------------------------------------------------ */
        
        inline explicit ustr(const char * us) : csl::common::var()
        {
          buf_.set("\0",1);
          if( !us ) return;
          // strlen only cares about trailing zero, so multibyte chars will not confuse here
          buf_.set( us, (::strlen(us)+1) );
        }

        inline ustr& operator=(const char * us)
        {
          if( !us ) return *this;
          // strlen only cares about trailing zero, so multibyte chars will not confuse here
          buf_.set( us, (::strlen(us)+1) );
          return *this;
        }

        inline friend ustr operator+(const char * lhs, const ustr& rhs)
        {
          return ustr(lhs) += rhs;
        }

        inline friend ustr operator+(const ustr& lhs, const char * rhs)
        {
          return ustr(lhs) += rhs;
        }

        ustr& operator+=(const char * s);

        inline bool operator==(const char * s) const
        {
          return (::strncmp( data(), s, static_cast<size_t>(nbytes()) ) == 0);
        }

        inline const char * c_str() const
        {
          return( data() );
        }

        inline ustr & assign(const char * start, const char * end)
        {
          buf_.set( start, end-start);
          return *this;
        }

        size_t find(const char * s) const;

        inline const char * data() const
        {
          return buf_.data();
        }

        bool from_string(const char * v);

        /* ------------------------------------------------------------------------ *
        **    wchar_t * operations
        ** ------------------------------------------------------------------------ */
        
        bool from_string(const wchar_t * v);

        /* ------------------------------------------------------------------------ *
        **    std::string operations
        ** ------------------------------------------------------------------------ */

        ustr& operator=(const std::string & s)
        {
          return operator=(s.c_str());
        }
        bool to_string(std::string & v) const;
        bool from_string(const std::string & v);

        /* ------------------------------------------------------------------------ *
        **    char operations
        ** ------------------------------------------------------------------------ */

        inline char operator[](const size_t n) const
        {
          return data()[n];
        }

        char at(const size_t n) const;
        size_t find(char w) const;
        size_t rfind(char w) const;

        /* ------------------------------------------------------------------------ *
        **    wchar_t operations
        ** ------------------------------------------------------------------------ */

        /* ------------------------------------------------------------------------ *
        **    int64, int64_t operations
        ** ------------------------------------------------------------------------ */
        
        int64 crc64() const;
        inline bool to_integer(int64 & v) const { return v.from_string(data()); }
        bool to_integer(int64_t & v) const;
        inline bool from_integer(const int64 & v) { return v.to_string(*this); }
        bool from_integer(int64_t v);

        /* ------------------------------------------------------------------------ *
        **    dbl, double operations
        ** ------------------------------------------------------------------------ */
        
        inline bool to_double(dbl & v) const { return v.from_string(data()); }
        bool to_double(double & v) const;
        inline bool from_double(const dbl & v) { return v.to_string(*this); }
        bool from_double(double v);
        
        /* ------------------------------------------------------------------------ *
        **    binry, binary operations
        ** ------------------------------------------------------------------------ */
        
        inline bool to_binary(binry & v) const { return v.from_binary(buf_.data(),buf_.size()); }
        bool to_binary(unsigned char * v, size_t & sz) const;
        bool to_binary(void * v, size_t & sz) const;
        inline bool from_binary(const binry & v) { return v.to_string(*this); }
        bool from_binary(const unsigned char * v,size_t sz);
        bool from_binary(const void * v,size_t sz);

        /* ------------------------------------------------------------------------ */

        inline void clear()
        {
          reset();
        }

        inline void reset()
        {
          buf_.reset();
          buf_.set( "\0",1 );
        }

        inline size_t size() const
        {
          /* I presume the trailing zero is already there */
          return (empty() ? 0 : nbytes()-1 );
        }

        inline size_t nbytes() const
        {
          return buf_.nbytes();
        }

        inline size_t nchars() const
        {
          // strlen() wouldn't do here, because of multibyte utf-8 characters
          return (empty() ? 0 : ::mbstowcs(NULL,data(),0));
        }

        inline bool empty() const
        {
          return (buf_.size() <= 1);
        }

        inline const preallocated_array<char,buf_size> & buffer() const
        {
          return buf_;
        }

        inline preallocated_array<char,buf_size> & buffer()
        {
          return buf_;
        }

        void ensure_trailing_zero();

        inline const unsigned char * ucharp_data() const
        {
          return reinterpret_cast<const unsigned char *>(buf_.data());
        }

        inline size_t var_size() const { return nbytes(); }

        /* ------------------------------------------------------------------------ *
        **  other conversion operations
        ** ------------------------------------------------------------------------ */
        
        inline bool to_var(var & v) const { return v.from_string(data()); }        
        bool from_var(const var & v) { return v.to_string(*this); }
        
        virtual inline void serialize(archiver & a) { a.serialize(*this); }

        inline ustr & operator+=(const var & other)
        {
          ustr tmp;
          tmp.from_var(other);
          return this->operator+=(tmp);
        }

      private:
        preallocated_array<char,buf_size>   buf_;
    };

    // append template
    template <typename ARG>
    ustr & operator<<(ustr & s, const ARG & arg)
    {
      return (s.operator+=(arg));
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_ustr_hh_included_ */
