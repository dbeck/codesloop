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

#ifndef _csl_common_simpstr_hh_included_
#define _csl_common_simpstr_hh_included_
#include "codesloop/common/stpodary.hh"
#include "codesloop/common/dbc.hh"
#include "codesloop/common/strconcat.hh"
#include <wctype.h>
#include <string>

namespace csl
{
  namespace common
  {
    class str
    {
      public:
        CSL_CLASS( csl::common::str );
        CSL_INVARIANT();
        CSL_DECLARE_EXCEPTION( conversion_error );
        CSL_DECLARE_EXCEPTION( out_of_memory );

        typedef wchar_t         elem_t;
        typedef const elem_t *  value_t;
        
        static const size_t buf_items = 128;
        static const size_t buf_size  = buf_items * sizeof(elem_t);
        static const size_t npos      = static_cast<size_t>(-1);
        
        typedef stpodary<elem_t,buf_items>  buf_t;

        virtual ~str() {}

        // constructors
        str();
        str(const str& s);
        explicit str(const char *);
        explicit str(const wchar_t * wcs);
        explicit str(const std::string & s);
                
        // operators
        str& operator=(const str& s);
        str& operator=(const char *);
        str& operator=(const wchar_t * wcs);
        str& operator=(const std::string & s);
        
        str& operator+=(const str& s);
        str& operator+=(const wchar_t * str);
        str& operator+=(const char * str);
        str& operator+=(const std::string & s);
        str& operator+=(const char c);
        str& operator+=(const wchar_t w);
        
        friend str operator+(const str& lhs, const str& rhs)
          { return str(lhs) += rhs; }

        friend str operator+(const wchar_t * lhs, const str& rhs)
          { return str(lhs) += rhs; }

        friend str operator+(const str& lhs, const wchar_t * rhs)
          { return str(lhs) += rhs; }

        bool operator==(const str& s) const;
        bool operator==(const char * s) const;
        bool operator==(const wchar_t * s) const;

        const str & operator>>(std::string & s) const;

        template <typename Other>
        inline bool operator!=(const Other* s) const
          { return !(*this == s); }
                        
        template <typename Other>
        inline bool operator!=(const Other& s) const
          { return !(*this == s); }

        inline wchar_t operator[](const size_t n) const
          { return (data())[n]; }

        // others
        wchar_t at(const size_t n) const;

        str substr(const size_t start, const size_t length) const;
        str trim() const;
                                                                                                                                                                                                
        size_t find(const str & s) const;
        size_t find(wchar_t w) const;
        size_t find(const wchar_t * s) const;
        size_t find(char c) const;
        size_t find(const char * s) const;
                
        size_t rfind(wchar_t w) const;

        str & assign(const wchar_t * start, const wchar_t * end);

        inline const wchar_t * c_str() const
          { return( data() ); } // TODO: have a char * version

        inline const str & to_std(std::string & s) const { return (*this >> s); }
        std::string to_std() const;

        inline const wchar_t * data() const { return buf_.data(); }

        inline void clear() { reset(); }
        void reset();

        inline size_t size() const
          { return (empty() ? 0 : (buf_.size()-1)); }
          // I presume the trailing zero is already there

        inline size_t nbytes() const { return buf_.nbytes();}
        inline size_t nchars() const
          {  return empty() ? 0 : ::wcstombs(NULL, data(), 0); }
          // wcstombs should take care of 'combining characters' too  

        inline bool empty() const { return (buf_.size() <= 1); }
        inline const buf_t & buffer() const { return buf_; }

      private:
        buf_t buf_;
    };

    // append template
    template <typename ARG>
    str & operator<<(str & s, const ARG & arg)
    {
      return (s.operator+=(arg));
    }
  }
}

#endif /* _csl_common_simpstr_hh_included_ */
