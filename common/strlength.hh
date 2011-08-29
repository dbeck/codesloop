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

#ifndef _csl_common_strlength_hh_included_
#define _csl_common_strlength_hh_included_
#include "codesloop/common/zero.hh"
#include "codesloop/common/dbc.hh"

namespace csl
{
  namespace common
  {
    template <typename T> struct strlength
    {
    };

    template <> struct strlength<char>
    {
      CSL_CLASS( csl::common::strlength );
      
      static size_t execute(const char * s)
      {
        CSL_REQUIRE( s != NULL );
        if( s == NULL ) { return 0; }
        return ::strlen(s);
      }

      size_t operator()(const char * s) const
      {
        return execute(s);
      }
    };

    template <> struct strlength<wchar_t>
    {
      CSL_CLASS( csl::common::strlength );
      
      static size_t execute(const wchar_t * s)
      {
        CSL_REQUIRE( s != NULL );
        if( s == NULL ) { return 0; }
        return ::wcslen(s);
      }

      size_t operator()(const wchar_t * s) const
      {
        return execute(s);
      }
    };
  }
}

#endif /* _csl_common_strlength_hh_included_ */

