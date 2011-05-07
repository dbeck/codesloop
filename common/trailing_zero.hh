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

#ifndef _csl_common_trailing_zero_hh_included_
#define _csl_common_trailing_zero_hh_included_
#include "codesloop/common/common.h"
#include "codesloop/common/dbc.hh"
#include "codesloop/common/excbase.hh"
#include "codesloop/common/zero.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <typename CollectionT> struct trailing_zero
    {
      CSL_CLASS( csl::common::trailing_zero );

      typedef typename CollectionT::elem_t value_t;
      
      static bool check(const CollectionT & collection)
      {
        CSL_REQUIRE( collection.size() > 0 );
        if( collection.size() > 0 )
        {
          if( collection[collection.size()-1] == zero<value_t>::val_ )
          {
            return true;
          }          
        }
        return false;
      }
      
      static void ensure(CollectionT & collection)
      {
        value_t last_item = zero<value_t>::val_;
        ++last_item;
        if( collection.size() > 0 )
        {
          last_item = collection[collection.size()-1];
        }
        
        if( last_item != zero<value_t>::val_ )
        {
          collection.append(&zero<value_t>::val_,1);
        }
        CSL_ENSURE( check(collection) == true );
      }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_trailing_zero_hh_included_ */

