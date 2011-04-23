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

#include "codesloop/common/common.h"
#include "codesloop/common/stpodary.hh"
#include "codesloop/common/dbc.hh"
#include "codesloop/common/excbase.hh"
#include <wctype.h>
#ifdef __cplusplus
#include <string>

namespace csl
{
  namespace common
  {
    class simpstr
    {
      public:
        CSL_CLASS( csl::common::simpstr );
        CSL_INVARIANT();
        CSL_DECLARE_EXCEPTION( conversion_error );
        CSL_DECLARE_EXCEPTION( out_of_memory );

        typedef wchar_t         elem_t;
        typedef const elem_t *  value_t;
        
        static const size_t buf_items = 128;
        static const size_t buf_size  = buf_items * sizeof(elem_t);
        static const size_t npos      = static_cast<size_t>(-1);
        
        typedef stpodary<elem_t,buf_items>  buf_t;

        simpstr();
        virtual ~simpstr() {}

        // ------------------------------------------------------------------------
        //    str operations

        simpstr(const simpstr& s);
        simpstr& operator=(const simpstr& s);
        simpstr& operator+=(const simpstr&);

        inline friend simpstr operator+(const simpstr& lhs, const simpstr& rhs)
        {
          return simpstr(lhs) += rhs;
        }

        bool operator==(const simpstr& s) const;
        size_t find(const simpstr & s) const;
        simpstr substr(const size_t start, const size_t length) const;
        simpstr trim() const;

        // ------------------------------------------------------------------------
        //    char * operations

        explicit simpstr(const char *);
        simpstr& operator=(const char *);
        bool operator==(const char * s) const;

        // ------------------------------------------------------------------------
        //    wchar_t * operations

        simpstr(const wchar_t * wcs);
        simpstr& operator=(const wchar_t * wcs);
        simpstr& operator+=(const wchar_t * str);

        inline friend simpstr operator+(const wchar_t * lhs, const simpstr& rhs)
        {
          return simpstr(lhs) += rhs;
        }

        inline friend simpstr operator+(const simpstr& lhs, const wchar_t * rhs)
        {
          return simpstr(lhs) += rhs;
        }

        bool operator==(const wchar_t * s) const;

        inline const wchar_t * c_str() const
        {
          return( data() ); // TODO: have a char * version
        }

        simpstr & assign(const wchar_t * start, const wchar_t * end);
        size_t find(const wchar_t * s) const;
        inline const wchar_t * data() const { return buf_.data(); }

        // ------------------------------------------------------------------------
        //    std::string operations

        simpstr(const std::string & s);
        simpstr& operator=(const std::string & s);

        // ------------------------------------------------------------------------
        //    wchar_t operations

        inline wchar_t operator[](const size_t n) const
        {
          return (data())[n];
        }

        wchar_t at(const size_t n) const;
        size_t find(wchar_t w) const;
        size_t rfind(wchar_t w) const;

        // ------------------------------------------------------------------------

        inline void clear() { reset(); }
        void reset();

        inline size_t size() const
        {
          // I presume the trailing zero is already there
          return (empty() ? 0 : (buf_.size()-1));
        }

        inline size_t nbytes() const { return buf_.nbytes();}

        inline size_t nchars() const
        {
          // wcstombs should take care of 'combining characters' too  
          return empty() ? 0 : ::wcstombs(NULL, data(), 0);
        }

        inline bool empty() const { return (buf_.size() <= 1); }

        inline const buf_t & buffer() const { return buf_; }

        void ensure_trailing_zero();

      private:
        buf_t buf_;
    };

    // append template
    template <typename ARG>
    simpstr & operator<<(simpstr & s, const ARG & arg)
    {
      return (s.operator+=(arg));
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_simpstr_hh_included_ */
