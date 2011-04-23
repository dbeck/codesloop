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

#include "codesloop/common/simpstr.hh"
#include "codesloop/common/common.h"

namespace csl
{
  namespace common
  {
    namespace
    {
      static uint64_t CRCTable[256];

      static int initalize_CRCTable()
      {
        int i, j;
        uint64_t part;

        for (i = 0; i < 256; i++)
        {
          part = i;
          for (j = 0; j < 8; j++)
          {
            if (part & 1)
              part = (part >> 1) ^ 0xd800000000000000ULL;
            else
              part >>= 1;
          }
          CRCTable[i] = part;
        }

        return 0;
      }
    }
    
#ifdef CSL_DEBUG
    bool simpstr::csl_invariant() const
    {
      // check for trailing zero
      if( buf_.size() == 0 ) return false;
      if( buf_[buf_.size()-1] != 0 ) return false;
      return true;
    }
#endif //CSL_DEBUG

    void simpstr::ensure_trailing_zero()
    {
      wchar_t c = 0;

      if( buf_.size() == 0 )
      {
        buf_.append( &c,1 );
      }
      else if( data()[buf_.size()-1] != c )
      {
        buf_.append( &c,1 );
      }
      CSL_CHECK_INVARIANT();
    }

    simpstr& simpstr::operator+=(const simpstr& s)
    {    
      if( s.empty() ) return *this;

      size_t sz = buf_.size();

      if( sz > 0 && data()[sz-1] == 0 )
      {
        // exclude the trailing zero character
        buf_.allocate( sz-1 );
      }

      buf_.append( s.buffer() );
      ensure_trailing_zero();
      
      CSL_CHECK_INVARIANT();

      return *this;
    }

    simpstr& simpstr::operator+=(const wchar_t * s)
    {
      CSL_REQUIRE( s != 0 );
      
      if( !s ) return *this;
      
      size_t sz = buf_.size();

      if( sz > 0 && data()[sz-1] == 0 )
      {
        // exclude the trailing zero character
        buf_.allocate( sz-1 );
      }

      buf_.append( s, (::wcslen(s)+1) );
      // I presume, this is not needed because of wcslen+1
      // ensure_trailing_zero();
      
      CSL_CHECK_INVARIANT();

      return *this;
    }

    simpstr::simpstr(const char * st) : buf_( L'\0' )
    {
      CSL_REQUIRE( st != 0 );
      
      if( !st ) return;

      size_t len = ::strlen(st)+1;
      size_t ssz = 0;

      wchar_t * nptr = buf_.allocate( len );
      
      if( !nptr )
      {
        CSL_THROW( out_of_memory );    
      }
      else if ( (ssz = ::mbstowcs( nptr, st, len )) != size_t(-1) )
      {
        // may need to shrink, when utf-8 chars occupy more than one character
        buf_.allocate( ssz );
        ensure_trailing_zero();
      }
      else
      {
        reset();
        CSL_THROW( conversion_error );
      }
      
      CSL_CHECK_INVARIANT();
    }

    simpstr& simpstr::operator=(const char * st)
    {
      CSL_REQUIRE( st != 0 );

      if( !st ) return *this;

      size_t len =  strlen(st)+1;
      size_t ssz = 0;

      wchar_t * nptr = buf_.allocate( len );
      
      if( !nptr )
      {
        CSL_THROW( out_of_memory );    
      }
      else if ( (ssz = ::mbstowcs( nptr, st, len )) != size_t(-1) )
      {
        // may need to shrink, when utf-8 chars occupy more than one character
        buf_.allocate( ssz );
        ensure_trailing_zero();
      }
      else
      {
        reset();
        CSL_THROW( conversion_error );
      }
      
      CSL_CHECK_INVARIANT();
      
      return *this;
    }

    bool simpstr::operator==(const wchar_t * s) const
    {
      if( !s ) { return empty(); }
      int ret = wcscmp( data(), s );
      return (ret == 0);
    }
    
    bool simpstr::operator==(const char * s) const
    {
      if( !s ) { return empty(); }
      simpstr rhs(s);
      return (*this == rhs);
    }
    
    bool simpstr::operator==(const simpstr& s) const
    {
      if( size() != s.size() ) return false;
      if( empty() ) return true;
      int ret = wcscmp( data(), s.data() );
      return (ret == 0);
    }

    size_t simpstr::find(wchar_t c) const
    {
      size_t ret = npos;
      size_t len = size();

      for ( size_t pos = 0; pos < len ; pos++ )
      {
        if ( (*this)[pos] == c ) {
          ret = pos;
          break;
        }
      }

      return ret;
    }

    size_t simpstr::rfind(wchar_t c) const
    {
      size_t ret = npos;
      size_t len = size();

      for ( size_t pos = len-1; pos >= 0 ; --pos )
      {
        if ( (*this)[pos] == c ) {
          ret = pos;
          break;
        }
      }

      return ret;
    }

    size_t simpstr::find(const simpstr & s) const
    {
      CSL_REQUIRE( s.empty() == false );
      const wchar_t * p = ::wcsstr( data(), s.data() );
      size_t ret = npos;
      if ( p != NULL ) { ret = p - data(); }
      return ret;
    }

    size_t simpstr::find(const wchar_t * strv) const
    {
      CSL_REQUIRE( strv != 0 );
      
      if( !strv )    return npos;
      if( empty() )  return npos;

      const wchar_t * res = 0;

      if( (res = ::wcsstr(data(), strv)) == NULL ) return npos;

      return (res-data());
    }

    wchar_t simpstr::at(const size_t n) const
    {
     
      size_t l = ::wcslen( data() );
      CSL_REQUIRE( n <= l );
      
      if( n <= l ) return data()[n];
      else         return 0;
    }

    simpstr::simpstr(const std::string & s)
    {
      *this = s;      
      CSL_CHECK_INVARIANT();
    }

    simpstr& simpstr::operator=(const std::string & s)
    {
      return operator=(s.c_str());
    }

/*
    int64 simpstr::crc64() const
    {
      int64 ret;
      uint64_t crc = 0x0000000000000000ULL;
      const unsigned char * seq = ucharp_data();

      while (*seq)
          crc = CRCTable[(crc ^ *seq++) & 0xff] ^ (crc >> 8);

      return int64(static_cast<int64_t>(crc));
    }
  */
  
    simpstr simpstr::substr(const size_t start, const size_t length) const
    {
      simpstr s;
      size_t len = length;
      size_t sz = size();
      
      CSL_REQUIRE( (start+length) <= sz );
      CSL_REQUIRE( start <= sz );

      // shrink length to fit in
      if ( sz < (length + start) )
      {
        len = sz - start;
      }

      if ( len > 0 )
      {
        // copy string
        s.buf_.set( data()+start,len );
        s.ensure_trailing_zero();
      }
      
      CSL_CHECK_INVARIANT();

      return s;
    }

    simpstr simpstr::trim() const
    {
      size_t start = npos, length = 0;

      for ( size_t pos = 0; pos < size() ; pos ++ )
      {
        if ( start == npos && !iswspace( (*this)[pos] )  )
                start = pos;
        else if ( start != npos && !iswspace( (*this)[pos] ) )
                length = pos - start + 1;
      }
      CSL_CHECK_INVARIANT();
      return substr(start,length);
    }
    
    simpstr::simpstr(const wchar_t * wcs) : buf_(L'\0')
    {
      CSL_REQUIRE( wcs != 0 );
      *this = wcs;
      CSL_CHECK_INVARIANT();
    }

    simpstr& simpstr::operator=(const wchar_t * wcs)
    {
      CSL_REQUIRE( wcs != 0 );
      if( !wcs ) return *this;
      // wcslen only cares about trailing zero, so combining characters will not confuse here
      buf_.set( wcs, ::wcslen(wcs)+1 );
      CSL_CHECK_INVARIANT();
      return *this;
    }
    
    simpstr& simpstr::assign(const wchar_t * start, const wchar_t * end)
    {
      buf_.set( start, end-start );
      ensure_trailing_zero();
      CSL_CHECK_INVARIANT();
      return *this;
    }

    void simpstr::reset()
    {
      buf_.reset();
      buf_.set( L"\0",1 );
      CSL_ENSURE( empty() == true );
      CSL_ENSURE( nbytes() > 0 );
      CSL_ENSURE( nchars() == 0 );
      CSL_CHECK_INVARIANT();
    }
    
    simpstr::simpstr() : buf_( L'\0' )
    {
      CSL_ENSURE( empty() == true );
      CSL_ENSURE( nbytes() > 0 );
      CSL_ENSURE( nchars() == 0 );
      CSL_CHECK_INVARIANT();
    }
    
    simpstr::simpstr(const simpstr& s) : buf_( L'\0' )
    {
      buf_ = s.buf_;
      CSL_ENSURE( empty() == s.empty() );
      CSL_ENSURE( nbytes() == s.nbytes() );
      CSL_ENSURE( nchars() == s.nchars() );
      CSL_CHECK_INVARIANT();
    }

    simpstr& simpstr::operator=(const simpstr& s)
    {
      buf_ = s.buf_;
      CSL_ENSURE( empty() == s.empty() );
      CSL_ENSURE( nbytes() == s.nbytes() );
      CSL_ENSURE( nchars() == s.nchars() );
      CSL_CHECK_INVARIANT();
      return *this;
    }
  };
};

AUTOEXEC( csl, common, initalize_crc_table, initalize_CRCTable );
// EOF

