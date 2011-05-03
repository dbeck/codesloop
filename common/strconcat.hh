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

#ifndef _csl_common_strconcat_hh_included_
#define _csl_common_strconcat_hh_included_
#include "codesloop/common/trailing_zero.hh"
#include "codesloop/common/dbc.hh"
#include "codesloop/common/excbase.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <typename CollectionT> struct strconcat
    {
      CSL_CLASS( csl::common::strconcat );
      CSL_DECLARE_EXCEPTION( out_of_memory );
      
      typedef typename CollectionT::elem_t value_t;
      
      static CollectionT & execute(CollectionT & lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        
        // fast path for empty rhs
        if( rhs.size() == 1 ) return lhs;

        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);
        
        if( p == NULL )
        {
          CSL_THROW( out_of_memory );
        }
        
        lhs.append( rhs );
        CSL_ENSURE( trailing_zero<CollectionT>::check(lhs) == true );
        return lhs;
      }
      
      CollectionT & operator()(CollectionT & lhs, const CollectionT & rhs)
      {
        return execute(lhs,rhs);
      }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_strconcat_hh_included_ */

