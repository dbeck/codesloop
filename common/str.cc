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

#include "codesloop/common/str.hh"
#include "codesloop/common/trailing_zero.hh"
#include "codesloop/common/strconcat.hh"

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
    bool str::csl_invariant() const
    {
      // check for trailing zero
      if( buf_.size() == 0 ) return false;
      if( buf_[buf_.size()-1] != 0 ) return false;
      return true;
    }
#endif //CSL_DEBUG

    str& str::operator+=(const str& s)
    {
      strconcat<buf_t>::execute(buf_,s.buf_);
      CSL_CHECK_INVARIANT();
      return *this;      
    }

    str& str::operator+=(const wchar_t * s)
    {
      CSL_REQUIRE( s != 0 );
      if( !s ) { return *this; }
      strconcat<buf_t>::execute(buf_,s);
      CSL_CHECK_INVARIANT();
      return *this;      
    }
    
    str& str::operator+=(const char * s)
    {
      CSL_REQUIRE( s != 0 );
      if( !s ) { return *this; }
      strconcat<buf_t>::execute(buf_,s);
      CSL_CHECK_INVARIANT();
      return *this;      
    }
    
    str& str::operator+=(const std::string & s)
    {
      strconcat<buf_t>::execute(buf_,s.c_str());
      CSL_CHECK_INVARIANT();
      return *this;      
    }
    
    str& str::operator+=(const char c)
    {
      CSL_REQUIRE( c != zero<char>::val_ );
      strconcat<buf_t>::execute(buf_,c);
      CSL_CHECK_INVARIANT();
      return *this;      
    }
    
    str& str::operator+=(const wchar_t w)
    {
      CSL_REQUIRE( w != zero<wchar_t>::val_ );
      strconcat<buf_t>::execute(buf_,w);
      CSL_CHECK_INVARIANT();
      return *this;      
    }

    std::string str::to_std() const
    {
      std::string ret;
      *this >> ret;
      return ret;
    }

    const str & str::operator>>(std::string & s) const
    {
      typedef stpodary<char,buf_items>  cbuf_t;
      cbuf_t result;
      strconcat<cbuf_t>::execute(result,c_str());
      s.assign(result.private_data());
      return *this;
    }

    str::str(const char * st) : buf_( L'\0' )
    {
      CSL_REQUIRE( st != 0 );
      
      if( !st ) { return; }

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
        trailing_zero<buf_t>::ensure(buf_);
      }
      else
      {
        reset();
        CSL_THROW( conversion_error );
      }
      
      CSL_CHECK_INVARIANT();
    }

    str& str::operator=(const char * st)
    {
      CSL_REQUIRE( st != 0 );
      if( !st ) { return *this; }
      reset();
      if( *st ) { strconcat<buf_t>::execute(buf_,st); }
      CSL_CHECK_INVARIANT();
      return *this;
    }

    bool str::operator==(const wchar_t * s) const
    {
      CSL_REQUIRE( s != 0 );
      if( !s ) { return empty(); }
      int ret = wcscmp( data(), s );
      return (ret == 0);
    }
    
    bool str::operator==(const char * s) const
    {
      CSL_REQUIRE( s != 0 );
      if( !s ) { return empty(); }
      str rhs(s);
      return (*this == rhs);
    }

    bool str::operator==(const str& s) const
    {
      CSL_REQUIRE( s.buf_.size() > 0 );
      if( size() != s.size() ) return false;
      if( empty() ) return true;
      int ret = wcscmp( data(), s.data() );
      return (ret == 0);
    }

    size_t str::find(wchar_t c) const
    {
      CSL_REQUIRE( c != zero<wchar_t>::val_ );
      
      if( c == zero<wchar_t>::val_ ) return npos;

      size_t ret = npos;
      size_t len = size();

      for ( size_t pos = 0; pos < len ; ++pos )
      {
        if ( (*this)[pos] == c ) {
          ret = pos;
          break;
        }
      }

      return ret;
    }

    size_t str::rfind(wchar_t c) const
    {
      CSL_REQUIRE( c != zero<wchar_t>::val_ );
      
      if( c == zero<wchar_t>::val_ ) return npos;

      size_t ret = npos;
      size_t len = buf_.size();
      
      if( len > 1 )
      {
        const wchar_t * dp = data()+len-1;
        for ( size_t pos = 0; pos <= len ; ++pos )
        {
          if ( *dp == c )
          {
            ret = len-pos-1;
            break;
          }
          --dp;
        }
      }

      return ret;
    }

    size_t str::find(const str & s) const
    {
      CSL_REQUIRE( s.empty() == false );
      if( s.empty() || empty() ) { return npos; }
      const wchar_t * p = ::wcsstr( data(), s.data() );
      size_t ret = npos;
      if ( p != NULL ) { ret = p - data(); }
      return ret;
    }

    size_t str::find(const wchar_t * strv) const
    {
      CSL_REQUIRE( strv != 0 );
      CSL_REQUIRE( *strv != zero<wchar_t>::val_ );
      
      if( !strv || *strv == zero<wchar_t>::val_ )  { return npos; }
      if( empty() )                                { return npos; }

      const wchar_t * res = 0;

      if( (res = ::wcsstr(data(), strv)) == NULL ) return npos;

      return (res-data());
    }
    
    size_t str::find(char c) const
    {
      CSL_REQUIRE( c != zero<char>::val_ );
      if( c == zero<char>::val_ ) { return npos; }
      char t[2] = { c, zero<char>::val_ };
      str x(t);
      return find(x);
    }
    
    size_t str::find(const char * s) const
    {
      CSL_REQUIRE( s != NULL );
      CSL_REQUIRE( *s != zero<char>::val_ );
      if( s == NULL || *s == zero<char>::val_ ) { return npos; }
      str x(s);
      return find(x);
    }

    wchar_t str::at(const size_t n) const
    {
      size_t l = ::wcslen( data() );
      CSL_REQUIRE( n <= l );
      
      if( n <= l ) return data()[n];
      else         return 0;
    }

    str::str(const std::string & s)
    {
      *this = s;      
      CSL_CHECK_INVARIANT();
    }

    str& str::operator=(const std::string & s)
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
  
    str str::substr(const size_t start, const size_t length) const
    {
      str s;
      size_t len = length;
      size_t sz = size();
      
      CSL_REQUIRE( (start+length) <= sz );
      CSL_REQUIRE( start <= sz );
      CSL_REQUIRE( length >  0 );
      
      if( length == 0 ) { return s; }

      // shrink length to fit in
      if ( sz < (length + start) )
      {
        len = sz - start;
      }

      if ( len > 0 )
      {
        // copy string
        s.buf_.set( data()+start,len );
        trailing_zero<buf_t>::ensure(s.buf_);
      }
      
      CSL_CHECK_INVARIANT();

      return s;
    }

    str str::trim() const
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
    
    str::str(const wchar_t * wcs) : buf_(L'\0')
    {
      CSL_REQUIRE( wcs != 0 );
      CSL_REQUIRE( *wcs != zero<wchar_t>::val_ );
      if( !wcs || *wcs == zero<wchar_t>::val_ ) { return; }
      *this = wcs;
      CSL_CHECK_INVARIANT();
    }

    str& str::operator=(const wchar_t * wcs)
    {
      CSL_REQUIRE( wcs != 0 );
      CSL_REQUIRE( *wcs != zero<wchar_t>::val_ );
      if( !wcs || *wcs == zero<wchar_t>::val_ ) { return *this; }
      // wcslen only cares about trailing zero, so combining characters will not confuse here
      buf_.set( wcs, ::wcslen(wcs)+1 );
      CSL_CHECK_INVARIANT();
      return *this;
    }
    
    str& str::assign(const wchar_t * start, const wchar_t * end)
    {
      CSL_REQUIRE( (end-start) >= 0 );
      CSL_REQUIRE( start != NULL );
      CSL_REQUIRE( end != NULL );
      CSL_REQUIRE( *start != zero<wchar_t>::val_ );
      if( start == NULL || end == NULL || *start == zero<wchar_t>::val_ ) { return *this; }
      buf_.set( start, end-start );
      trailing_zero<buf_t>::ensure(buf_);
      CSL_CHECK_INVARIANT();
      return *this;
    }

    void str::reset()
    {
      buf_.reset();
      buf_.set( L"\0",1 );
      CSL_ENSURE( empty() == true );
      CSL_ENSURE( nbytes() > 0 );
      CSL_ENSURE( nchars() == 0 );
      CSL_CHECK_INVARIANT();
    }
    
    str::str() : buf_( L'\0' )
    {
      CSL_ENSURE( empty() == true );
      CSL_ENSURE( nbytes() > 0 );
      CSL_ENSURE( nchars() == 0 );
      CSL_CHECK_INVARIANT();
    }
    
    str::str(const str& s) : buf_( L'\0' )
    {
      buf_ = s.buf_;
      CSL_ENSURE( empty() == s.empty() );
      CSL_ENSURE( nbytes() == s.nbytes() );
      CSL_ENSURE( nchars() == s.nchars() );
      CSL_CHECK_INVARIANT();
    }

    str& str::operator=(const str& s)
    {
      CSL_REQUIRE( this != &s );
      // not checking equality to other string because that is already handled
      // by stpodary, the above requirement would also catch it in debug mode
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

