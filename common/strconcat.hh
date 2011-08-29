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
#include "codesloop/common/strlength.hh"

namespace csl
{
  namespace common
  {
    namespace strconcat_internal
    {
      template <typename T> struct sibling_type {};
      
      // convert functions are unchecked by intention, expecting
      // that strconcat do the neccesary checks instead

      template <> struct sibling_type<char>
      {
        CSL_CLASS( csl::common::strconcat_internal::sibling_type );
        CSL_DECLARE_EXCEPTION( out_of_memory );
        CSL_DECLARE_EXCEPTION( conversion_error );

        typedef wchar_t sibling_t;

        template <typename CollectionT>
        static CollectionT & convert( CollectionT & lhs, const wchar_t * rhs )
        {
          // over allocating takes care of combining charachters too
          size_t rlen = strlength<wchar_t>::execute( rhs );
          size_t allocd = 1+4*rlen;
          
          char * b = lhs.allocate( allocd );
          
          if( b == NULL )
          {
            CSL_THROW( out_of_memory );
          }
          
          size_t ssz = ::wcstombs( b, rhs, rlen+1 );
          
          if( ssz == static_cast<size_t>(-1) )
          {
            lhs.reset();
            CSL_THROW( conversion_error );
          }
          else if( ssz != rlen )
          {
            lhs.allocate( ssz );
          }
                    
          trailing_zero<CollectionT>::ensure( lhs );
          return lhs;
        }        
      };
      
      template <> struct sibling_type<wchar_t>
      {
        CSL_CLASS( csl::common::strconcat_internal::sibling_type );
        CSL_DECLARE_EXCEPTION( out_of_memory );
        CSL_DECLARE_EXCEPTION( conversion_error );

        typedef char sibling_t;
        
        template <typename CollectionT>
        static CollectionT & convert( CollectionT & lhs, const char * rhs )
        {
          // over allocating may happend due to the combining chars
          size_t rlen = strlength<char>::execute( rhs );
          
          wchar_t * b = lhs.allocate( 1+rlen );
          
          if( b == NULL )
          {
            CSL_THROW( out_of_memory );
          }
          
          size_t ssz = ::mbstowcs( b, rhs, rlen+1 );
          
          if( ssz == static_cast<size_t>(-1) )
          {
            lhs.reset();
            CSL_THROW( conversion_error );
          }
          else if( ssz != rlen )
          {
            lhs.allocate( ssz );
          }
                    
          trailing_zero<CollectionT>::ensure( lhs );
          return lhs;
        }
      }; 
    }
    
    template <typename CollectionT> struct strconcat
    {
      CSL_CLASS( csl::common::strconcat );
      CSL_DECLARE_EXCEPTION( out_of_memory );
      
      typedef typename CollectionT::elem_t                        value_t;
      typedef typename strconcat_internal::sibling_type<value_t>  sibling_type_t;
      typedef typename sibling_type_t::sibling_t                  sibling_t;
      
      static CollectionT & execute(CollectionT & lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        CSL_REQUIRE( lhs.size() != 0 );
        CSL_REQUIRE( rhs.size() != 0 );
                
        // parameter checks
        if( lhs.size() == 0 ) { trailing_zero<CollectionT>::ensure(lhs); }
                
        // empty rhs
        if( rhs.size() <= 1 ) return lhs;

        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);
        
        if( p == NULL ) { CSL_THROW( out_of_memory ); }
        
        lhs.append( rhs );
        
        if( &lhs == &rhs )
        {
          trailing_zero<CollectionT>::ensure( lhs );
        }

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
        CSL_REQUIRE( lhs.size() != 0 );

        // parameter checks
        if( lhs.size() == 0 ) { trailing_zero<CollectionT>::ensure(lhs); }
        if( rhs == NULL )     { return lhs; }
        
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
        CSL_REQUIRE( lhs.size() > 0 );
       
        if( lhs.size() == 0 ) { trailing_zero<CollectionT>::ensure(lhs); }

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
                  
      static CollectionT & execute(CollectionT & lhs, const sibling_t * rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( rhs != NULL );
        CSL_REQUIRE( lhs.size() > 0 );

        // parameter checks
        if( lhs.size() == 0 ) { trailing_zero<CollectionT>::ensure(lhs); }
        if( rhs == NULL )     { return lhs; }

        // empty string on rhs
        if( *rhs == zero<sibling_t>::val_ ) return lhs;
        
        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);

        if( p == NULL ) { CSL_THROW( out_of_memory ); }

        // convert rhs to value_t collection        
        CollectionT tmp;
        sibling_type_t::convert( tmp, rhs );
        
        lhs.append( tmp );
        
        CSL_ENSURE( trailing_zero<CollectionT>::check(lhs) == true );
        return lhs;
      }
      
      CollectionT & operator()(CollectionT & lhs, const sibling_t * rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT & execute(CollectionT & lhs, const sibling_t rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(lhs) == true );
        CSL_REQUIRE( lhs.size() > 0 );

        if( lhs.size() == 0 ) { trailing_zero<CollectionT>::ensure(lhs); }

        // terminator on rhs
        if( rhs == zero<sibling_t>::val_ ) return lhs;

        sibling_t rstr[2] = { rhs, zero<sibling_t>::val_ };
        
        // remove trailing zero
        value_t * p = lhs.allocate(lhs.size()-1);

        if( p == NULL ) { CSL_THROW( out_of_memory ); }

        // convert rhs to value_t collection        
        CollectionT tmp;
        sibling_type_t::convert( tmp, rstr );
        
        lhs.append( tmp );
        
        CSL_ENSURE( trailing_zero<CollectionT>::check(lhs) == true );
        return lhs;
      }

      CollectionT & operator()(CollectionT & lhs, const sibling_t rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT execute(const value_t * lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( lhs != NULL );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        CSL_REQUIRE( rhs.size() > 0 );
        
        CollectionT ret, rtmp;
        const CollectionT * rtmpp = &rhs;
        
        if( rhs.size() == 0 )
        {
          trailing_zero<CollectionT>::ensure(rtmp);
          rtmpp = &rtmp;
        }
          
        if( lhs == NULL || *lhs == zero<value_t>::val_ )
        {
          ret = *rtmpp;
        }
        else
        {        
          trailing_zero<CollectionT>::ensure(ret);
          execute( ret, lhs );        
          execute( ret, *rtmpp );
        }

        CSL_ENSURE( trailing_zero<CollectionT>::check(ret) == true );                        
        return ret;
      }
      
      CollectionT operator()(const value_t * lhs, const CollectionT & rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT execute(const value_t lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        CSL_REQUIRE( rhs.size() > 0 );
        
        CollectionT ret, rtmp;
        const CollectionT * rtmpp = &rhs;
        
        if( rhs.size() == 0 )
        {
          trailing_zero<CollectionT>::ensure(rtmp);
          rtmpp = &rtmp;
        }        
        
        if( lhs == zero<value_t>::val_ )
        {
          ret = *rtmpp;
        }
        else
        {
          ret.append( &lhs, 1 );
          ret.append( *rtmpp );
        }
        
        CSL_ENSURE( trailing_zero<CollectionT>::check(ret) == true );                        
        return ret;
      }

      CollectionT operator()(const value_t lhs, const CollectionT & rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT execute(const sibling_t * lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( lhs != NULL );
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );
        
        CollectionT ret, rtmp;
        const CollectionT * rtmpp = &rhs;
        
        if( rhs.size() == 0 )
        {
          trailing_zero<CollectionT>::ensure(rtmp);
          rtmpp = &rtmp;
        }
          
        if( lhs == NULL || *lhs == zero<sibling_t>::val_ )
        {
          ret = *rtmpp;
        }
        else
        {        
          trailing_zero<CollectionT>::ensure(ret);
          execute( ret, lhs );        
          execute( ret, *rtmpp );
        }

        CSL_ENSURE( trailing_zero<CollectionT>::check(ret) == true );                        
        return ret;
      }
      
      CollectionT operator()(const sibling_t * lhs, const CollectionT & rhs) const
      {
        return execute(lhs,rhs);
      }

      static CollectionT execute(const sibling_t lhs, const CollectionT & rhs)
      {
        CSL_REQUIRE( trailing_zero<CollectionT>::check(rhs) == true );

        CollectionT ret, rtmp;
        const CollectionT * rtmpp = &rhs;
        
        if( rhs.size() == 0 )
        {
          trailing_zero<CollectionT>::ensure(rtmp);
          rtmpp = &rtmp;
        }        

        if( lhs == zero<sibling_t>::val_ )
        {
          ret = *rtmpp;
        }
        else
        {          
          trailing_zero<CollectionT>::ensure(ret);
          sibling_t lstr[2] = { lhs, zero<sibling_t>::val_ };
          execute( ret, lstr );
          execute( ret, *rtmpp );
        }
                
        CSL_ENSURE( trailing_zero<CollectionT>::check(ret) == true );                        
        return ret;
      }
      
      CollectionT operator()(const sibling_t lhs, const CollectionT & rhs) const
      {
        return execute(lhs,rhs);
      }      
    };
  }
}

#endif /* _csl_common_strconcat_hh_included_ */

