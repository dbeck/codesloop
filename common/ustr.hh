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

Credits: some techniques and code pieces are stolen from Christian
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

        /** @brief constructor */
        inline ustr() : csl::common::var()
        {
          buf_.set("\0",1);
        }

        /** @brief destructor */
        virtual ~ustr() {}

        /* ------------------------------------------------------------------------ *
        **    str operations
        ** ------------------------------------------------------------------------ */

        /** @brief copy constructor */
        explicit ustr(const str & other);

        /** @brief copy operator */
        ustr & operator=(const str & other);

        /** @brief append operator */
        ustr& operator+=(const str&);

        /** @brief is equal operator */
        inline bool operator==(const str& s) const
        {
          ustr rhs(s);
          return (*this == rhs);
        }

        /* ------------------------------------------------------------------------ *
        **    ustr operations
        ** ------------------------------------------------------------------------ */

        /** @brief copy constructor */
        inline ustr(const ustr& s) : csl::common::var()
        {
          buf_.set("\0",1);
          buf_ = s.buf_;
        }

        /** @brief copy operator */
        inline ustr& operator=(const ustr& s)
        {
          buf_ = s.buffer();
          return *this;
        }

        /** @brief append operator */
        ustr& operator+=(const ustr&);

        /**
        @brief append operator with two parameters
        @param lhs is the left-hand-side of add
        @param rhs is the right-hand-side of add

        adds lhs+rhs and returns a fresh new result string
         */
        inline friend ustr operator+(const ustr& lhs, const ustr& rhs)
        {
          return ustr(lhs) += rhs;
        }

        /** @brief is equal operator */
        inline bool operator==(const ustr& s) const
        {
          return (::strncmp( data(), s.data(), static_cast<size_t>(nbytes()) ) == 0);
        }

        /**
        @brief find a substring of string
        @param s is the substring to be found
        @returns npos if not found or the position
         */
        size_t find(const ustr & s) const;

        /**
        @brief extracts a substring from a given position
        @param start start from this position
        @param length is the amount to be extracted
         */
        ustr substr(const size_t start, const size_t length) const;

        /**
        @brief trim spaces from string start and end positions
        */
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

        /* ------------------------------------------------------------------------ *
        **    char * operations
        ** ------------------------------------------------------------------------ */

        /** @brief copy constructor */
        inline explicit ustr(const char * us) : csl::common::var()
        {
          buf_.set("\0",1);
          if( !us ) return;
          // strlen only cares about trailing zero, so multibyte chars will not confuse here
          buf_.set( us, (::strlen(us)+1) );
        }

        /** @brief copy operator */
        inline ustr& operator=(const char * us)
        {
          if( !us ) return *this;
          // strlen only cares about trailing zero, so multibyte chars will not confuse here
          buf_.set( us, (::strlen(us)+1) );
          return *this;
        }

        /**
        @brief append operator with two parameters
        @param lhs is the left-hand-side of add
        @param rhs is the right-hand-side of add

        adds lhs+rhs and returns a fresh new result string
         */
        inline friend ustr operator+(const char * lhs, const ustr& rhs)
        {
          return ustr(lhs) += rhs;
        }

        /**
        @brief append operator with two parameters
        @param lhs is the left-hand-side of add
        @param rhs is the right-hand-side of add

        adds lhs+rhs and returns a fresh new result string
         */
        inline friend ustr operator+(const ustr& lhs, const char * rhs)
        {
          return ustr(lhs) += rhs;
        }

        /** @brief append operator */
        ustr& operator+=(const char * s);

        /** @brief is equal operator */
        inline bool operator==(const char * s) const
        {
          return (::strncmp( data(), s, static_cast<size_t>(nbytes()) ) == 0);
        }

        /** @brief returns the background c str */
        inline const char * c_str() const
        {
          return( data() );
        }

        /**
        @brief assign string
        @param start is the start of the string
        @param lend is the end of the string
         */
        inline ustr & assign(const char * start, const char * end)
        {
          buf_.set( start, end-start);
          return *this;
        }

        /**
        @brief find a wide character in the string
        @param s is the string to be found
        @returns npos if not found or the position
         */
        size_t find(const char * s) const;

        /** @brief get data as char * */
        inline const char * data() const
        {
          return buf_.data();
        }

        /* ------------------------------------------------------------------------ *
        **    wchar_t * operations
        ** ------------------------------------------------------------------------ */

        /* ------------------------------------------------------------------------ *
        **    std::string operations
        ** ------------------------------------------------------------------------ */

        /** @brief let equal operator */
        ustr& operator=(const std::string & s)
        {
          return operator=(s.c_str());
        }

        /* ------------------------------------------------------------------------ *
        **    char operations
        ** ------------------------------------------------------------------------ */

        /** @brief unchecked access to buffer */
        inline char operator[](const size_t n) const
        {
          return data()[n];
        }

        /** @brief check access to buffer */
        char at(const size_t n) const;

        /**
        @brief find a wide character in the string
        @param w is the character to be found
        @returns npos if not found or the position
         */
        size_t find(char w) const;

        /**
        @brief reverse find a wide character in the string
        @param w is the character to be found
        @returns npos if not found or the position
         */
        size_t rfind(char w) const;

        /* ------------------------------------------------------------------------ *
        **    wchar_t operations
        ** ------------------------------------------------------------------------ */

        /* ------------------------------------------------------------------------ *
        **    int64  operations
        ** ------------------------------------------------------------------------ */
        /**
        @brief generates crc64 hash value from string
        @returns signed 64 bit integer with CRC
        */
        int64 crc64() const;

        /* ------------------------------------------------------------------------ */

        /** @brief resets ustr buffer */
        inline void clear()
        {
          reset();
        }

        /** @brief reset internal data */
        inline void reset()
        {
          buf_.reset();
          buf_.set( "\0",1 );
        }

        /** @brief gets ustr size  */
        inline size_t size() const
        {
          /* I presume the trailing zero is already there */
          return (empty() ? 0 : nbytes()-1 );
        }

        /** @brief gets the total allocated bytes */
        inline size_t nbytes() const
        {
          return buf_.size();
        }

        /** @brief return the number of characters in the string, excluding the trailing zero */
        inline size_t nchars() const
        {
          // strlen() wouldn't do here, because of multibyte utf-8 characters
          return (empty() ? 0 : ::mbstowcs(NULL,data(),0));
        }

        /**
        @brief checks if string has data
        @return true if the string is empty
        */
        inline bool empty() const
        {
          return (buf_.size() <= 1);
        }

        /** @brief returns the internal buffer that stores the string */
        inline const preallocated_array<char,buf_size> & buffer() const
        {
          return buf_;
        }

        /** @brief returns the internal buffer that stores the string */
        inline preallocated_array<char,buf_size> & buffer()
        {
          return buf_;
        }

        /**
        @brief ensures that the string has a trailing zero

        this is mainly called internally. member functions use this, but users are free
        to call as well. member functions are expected to make sure that a trailing zero
        exists so it makes little sense for users, to call this function.
         */
        void ensure_trailing_zero();

        /** @brief returns a const pointer to internal data */
        inline const unsigned char * ucharp_data() const
        {
          return reinterpret_cast<const unsigned char *>(buf_.data());
        }

        /** @brief returns the size of the variable data */
        inline size_t var_size() const { return buf_.size(); }

        /* ------------------------------------------------------------------------ *
        **    conversion operations
        ** ------------------------------------------------------------------------ */

        inline bool to_integer(int64 & v) const { return v.from_string(data()); }
        bool to_integer(int64_t & v) const;
        inline bool to_double(dbl & v) const { return v.from_string(data()); }
        bool to_double(double & v) const;
        inline bool to_string(str & v) const { return v.from_string(data()); }
        inline bool to_string(ustr & v) const { v.buf_ = buf_; return true; }
        bool to_string(std::string & v) const;
        inline bool to_binary(binry & v) const { return v.from_binary(buf_.data(),buf_.size()); }
        bool to_binary(unsigned char * v, size_t & sz) const;
        bool to_binary(void * v, size_t & sz) const;
        bool to_xdr(xdrbuf & b) const;
        inline bool to_var(var & v) const { return v.from_string(data()); }
        inline bool from_integer(const int64 & v) { return v.to_string(*this); }
        bool from_integer(int64_t v);
        inline bool from_double(const dbl & v) { return v.to_string(*this); }
        bool from_double(double v);
        inline bool from_string(const str & v) { *this = v; return true; }
        inline bool from_string(const ustr & v) { buf_ = v.buf_; return true; }
        bool from_string(const std::string & v);
        bool from_string(const char * v);
        bool from_string(const wchar_t * v);
        inline bool from_binary(const binry & v) { return v.to_string(*this); }
        bool from_binary(const unsigned char * v,size_t sz);
        bool from_binary(const void * v,size_t sz);
        bool from_xdr(xdrbuf & v);
        bool from_var(const var & v) { return v.to_string(*this); }
        virtual inline void serialize(arch & buf) { buf.serialize(*this); }

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
