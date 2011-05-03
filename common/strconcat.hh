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
#include "codesloop/common/strlength.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    namespace strconcat_internal
    {
      template <typename T> struct sibling_type {};
      template <> struct sibling_type<char>     { typedef wchar_t sibling_t; };
      template <> struct sibling_type<wchar_t>  { typedef char sibling_t; }; 
    }
    
    template <typename CollectionT> struct strconcat
    {
      CSL_CLASS( csl::common::strconcat );
      CSL_DECLARE_EXCEPTION( out_of_memory );
      CSL_DECLARE_EXCEPTION( invalid_parameter );
      
      typedef typename CollectionT::elem_t value_t;
      typedef typename strconcat_internal::sibling_type<value_t>::sibling_t sibling_t;
      
      static CollectionT & execute(CollectionT & lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        
        // parameter checks
        if( lhs.size() == 0 ) { CSL_THROW( invalid_parameter ); }
        if( rhs.size() == 0 ) { CSL_THROW( invalid_parameter ); }
        
        // empty rhs
        if( rhs.size() == 1 ) return lhs;

        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);
        
        if( p == NULL ) { CSL_THROW( out_of_memory ); }
        
        lhs.append( rhs );
        CSL_ENSURE( trailing_zero<CollectionT>::check(lhs) == true );
        return lhs;
      }

      CollectionT & operator()(CollectionT & lhs, const CollectionT & rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT & execute(CollectionT & lhs, const value_t * rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( rhs != NULL );

        // parameter checks
        if( lhs.size() == 0 ) { CSL_THROW( invalid_parameter ); }
        if( rhs == NULL )     { CSL_THROW( invalid_parameter ); }
        
        // empty string on rhs
        if( *rhs == zero<value_t>::val_ ) return lhs;
        
        size_t rlen = strlength<value_t>::execute(rhs);
        
        if( rlen == 0 ) return lhs;
        
        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);
        
        if( p == NULL ) { CSL_THROW( out_of_memory ); }
         
        lhs.append( rhs, (rlen+1) );
        CSL_ENSURE( trailing_zero<CollectionT>::check(lhs) == true );

        return lhs;
      }

      CollectionT & operator()(CollectionT & lhs, const value_t * rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT & execute(CollectionT & lhs, const value_t rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
       
        if( lhs.size() == 0 ) { CSL_THROW( invalid_parameter ); }

        // terminator on rhs
        if( rhs == zero<value_t>::val_ ) return lhs;

        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);

        if( p == NULL ) { CSL_THROW( out_of_memory ); }
        
        value_t rstr[2] = { rhs, zero<value_t>::val_ };

        lhs.append( rstr, 2 );
        CSL_ENSURE( trailing_zero<CollectionT>::check(lhs) == true );

        return lhs;
      }

      CollectionT & operator()(CollectionT & lhs, const value_t rhs) const
      {
        return execute(lhs,rhs);
      }
      
      // -------------------
                  
      static CollectionT & execute(CollectionT & lhs, const sibling_t * rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( rhs != NULL );

        // TODO
        return lhs;
      }

      static CollectionT & execute(CollectionT & lhs, const sibling_t rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        
        // TODO
        return lhs;
      }
      
      // -------------------

      static CollectionT execute(const value_t * lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( lhs != NULL );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        
        // TODO
        return CollectionT();
      }

      static CollectionT execute(const value_t lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );

        // TODO
        return CollectionT();
      }

      static CollectionT execute(const sibling_t * lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( lhs != NULL );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        
        // TODO
        return CollectionT();
      }

      static CollectionT execute(const sibling_t lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );

        // TODO
        return CollectionT();
      }                         
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_strconcat_hh_included_ */

