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

#include "codesloop/common/lgr_msg.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/strlength.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/stpodary.hh"

namespace
{
  using csl::common::trailing_zero;
  using csl::common::strconcat;
  using csl::common::strlength;
  using csl::common::lgr::loc;
  using csl::common::lgr::locs;

  typedef csl::common::stpodary<wchar_t,2048> result_t;
  typedef csl::common::stpodary<char,1024>    str_tmp_t;
  typedef csl::common::stpodary<wchar_t,1024> wstr_tmp_t;

  void strip_trailing_zero( result_t & res )
  {
    if( res.size() > 0 && trailing_zero<result_t>::check(res) )
    {
      res.allocate( res.size()-1 );
    }
  }

  void conv_data( result_t & res, bool val )
  {
    if( val ) res.append( L"true", 4 );
    else      res.append( L"false", 5 );
  }

  void conv_data( result_t & res, int64_t val )
  {
    wchar_t tmp[28];
    int len = swprintf(tmp,27, L"%lld",val );
    if( len != -1 ) res.append( tmp,len );
  }

  void conv_data( result_t & res, double val )
  {
    wchar_t tmp[28];
    int len = swprintf(tmp,27, L"%.6f",val );
    if( len != -1 ) res.append( tmp,len );
  }

  void conv_data( result_t & res, const char * s, size_t len )
  {
    trailing_zero<result_t>::ensure(res);
    strconcat<result_t>::execute(res,s,len);
    strip_trailing_zero( res );
  }

  void conv_data( result_t & res, const char * s )
  {
    trailing_zero<result_t>::ensure(res);
    strconcat<result_t>::execute(res,s);
    strip_trailing_zero( res );
  }

  void conv_data( result_t & res, const wchar_t * s, size_t len )
  {
    trailing_zero<result_t>::ensure(res);
    strconcat<result_t>::execute(res,s,len);
    strip_trailing_zero( res );
  }

  void conv_data( result_t & res, const wchar_t * s )
  {
    trailing_zero<result_t>::ensure(res);
    strconcat<result_t>::execute(res,s);
    strip_trailing_zero( res );
  }

  void conv_locid( result_t & res, unsigned int locid )
  {
    loc & l(locs::instance().get(locid));
    /*conv_data(res,L"\n  Location: ");*/ conv_data(res,l.wfile());  conv_data(res,L":"); conv_data(res,static_cast<int64_t>(l.line()));
    conv_data(res,L" ");
    /*conv_data(res,L"\n  Function: ");*/ conv_data(res,l.wclazz()); conv_data(res,L"::"); conv_data(res,l.func());
    conv_data(res,L" ");
    conv_data(res,csl::common::logger_base::level_to_wstring(l.level()));
    conv_data(res,L" ");
  }

  void conv_start( result_t & res ) { conv_data(res,L"\nlog {"); }
  void conv_end( result_t & res )   { conv_data(res,L"\n}\n"); }

  // get_data<uint32_t>(encoded,len,locid,pos);

  template <typename T>
  void get_data( const uint8_t * ptr, size_t max, T & val, size_t & pos )
  {
    size_t needed = sizeof(T);
    if( (pos+needed) <= max )
    {
      ::memcpy( &val, (ptr+pos), needed );
      pos += needed;
    }
  }

  void get_data( const uint8_t * ptr, size_t max, void * collection, size_t clen, size_t & pos )
  {
    if( (pos+clen) <= max )
    {
      ::memcpy( collection, (ptr+pos), clen );
      pos += clen;
    }
  }

}

namespace csl
{
  namespace common
  {
    namespace lgr
    {
      msg::msg(lgr::loc & l, logger_base & lb) :
          loc_(&l),
          logger_(&lb),
          data_(lb.construct_msgdata()),
          used_(0),
          eor_seen_(false),
          skip_(true)
      {
        uint32_t li = loc_->locid();
        skip_ = (lock_.assign(data_.get_lock()) == false);
        if( !skip_ ) append(type_select<location_id>::sel_,&li,sizeof(li));
      }

      msg::~msg()
      {
        if( loc_ && loc_->level() == logger_base::scoped_ && eor_seen_ && skip_ == false )
        {
          data_ = logger_->construct_msgdata();
          skip_ = (lock_.assign(data_.get_lock()) == false);
          uint32_t li = loc_->locid();
          if( !skip_ )
          {
            append(type_select<location_id>::sel_,&li,sizeof(li));
            *this << return_from_function();
          }
        }
      }

      void msg::flush()
      {
        CSL_REQUIRE( skip_ == false );
        CSL_REQUIRE( data_.is_valid() );
        if( !skip_ && data_.is_valid() )
        {
          lock_.reset();
          common::ksmsg m(
              common::ksmsg::buf(data_.buf(),used_),
              data_.get_lock());
          data_.clear();
          used_ = 0;
          logger_->push(m);
        }
      }

      void msg::append(const void * data, size_t len)
      {
        CSL_REQUIRE( data != NULL );
        CSL_REQUIRE( len != 0 );
        CSL_REQUIRE( skip_ == false );
        if( skip_ ) return;
        long remaining = data_.len() - used_;
        if( remaining > static_cast<long>(len) )
        {
          ::memcpy(data_.buf()+used_,data,len);
          used_ += len;
        }
      }

      msg & msg::operator<<(const char * str)
      {
        CSL_REQUIRE( str != NULL );
        // append type
        append<uint8_t>(type_select<const char *>::sel_);
        // append length
        unsigned short sz = static_cast<unsigned short>(strlength<char>::execute(str));
        append<uint16_t>(sz);
        // append data
        if( sz > 0 ) append(str,sz);
        return *this;
      }

      msg & msg::operator<<(const wchar_t * str)
      {
        CSL_REQUIRE( str != NULL );
        // append type
        append<uint8_t>(type_select<const wchar_t *>::sel_);
        // append length
        unsigned short sz = static_cast<unsigned short>(strlength<wchar_t>::execute(str));
        append<uint16_t>(sz);
        // append data
        if( sz > 0 ) append(str,sz*sizeof(wchar_t));
        return *this;
      }

      msg & msg::operator<<(const csl::common::str & s)
      {
        CSL_REQUIRE( !s.empty() );
        // append type
        append<uint8_t>(type_select<const wchar_t *>::sel_);
        // append length
        unsigned short sz = static_cast<unsigned short>(s.size());
        append<uint16_t>(sz);
        // append data
        if( sz > 0 ) append(s.c_str(),sz*sizeof(wchar_t));
        return *this;
      }

#ifndef CSL_TYPE_SEL
#define CSL_TYPE_SEL(TYPE) type_select<TYPE>::val_
#endif // CSL_TYPE_SEL

#ifndef CSL_CONV_DATA
#define CSL_CONV_DATA(TYPE) \
      { \
        TYPE tmp_val = 0; \
        get_data<TYPE>(encoded,len,tmp_val,pos); \
        /*conv_data(res,L"\n  " #TYPE ": "); conv_data(res,static_cast<int64_t>(tmp_val));*/ \
        conv_data(res2,static_cast<int64_t>(tmp_val)); \
      }
#endif // CSL_CONV_DATA

#ifndef CSL_CONV_DATA2
#define CSL_CONV_DATA2(TYPE,CAST) \
      { \
        TYPE tmp_val = 0; \
        get_data<TYPE>(encoded,len,tmp_val,pos); \
        /*conv_data(res,L"\n  " #TYPE ": "); conv_data(res,static_cast<CAST>(tmp_val));*/ \
        conv_data(res2,static_cast<CAST>(tmp_val)); \
      }
#endif // CSL_CONV_DATA2


      void msg::to_str(const uint8_t * encoded, size_t len, str & result)
      {
        result_t    res,res2;
        str_tmp_t   stmp;
        wstr_tmp_t  wstmp;

        size_t pos = 0;
        uint32_t locid = 0;
        uint8_t typ = 0;

        res.append(L'\0');

        //conv_start(res);

        while(pos < len)
        {

          get_data<uint8_t>(encoded,len,typ,pos);

          switch(typ)
          {
          case CSL_TYPE_SEL(bool):      CSL_CONV_DATA2(bool,bool); break;
          case CSL_TYPE_SEL(int8_t):    CSL_CONV_DATA(int8_t); break;
          case CSL_TYPE_SEL(int16_t):   CSL_CONV_DATA(int16_t); break;
          case CSL_TYPE_SEL(int32_t):   CSL_CONV_DATA(int32_t); break;
          case CSL_TYPE_SEL(int64_t):   CSL_CONV_DATA(int64_t); break;
          case CSL_TYPE_SEL(uint8_t):   CSL_CONV_DATA(uint8_t); break;
          case CSL_TYPE_SEL(uint16_t):  CSL_CONV_DATA(uint16_t); break;
          case CSL_TYPE_SEL(uint32_t):  CSL_CONV_DATA(uint32_t); break;
          case CSL_TYPE_SEL(uint64_t):  CSL_CONV_DATA(uint64_t); break;

          case CSL_TYPE_SEL(float):     CSL_CONV_DATA2(float,double); break;
          case CSL_TYPE_SEL(double):    CSL_CONV_DATA2(double,double); break;
          case CSL_TYPE_SEL(tag): break;
          case CSL_TYPE_SEL(name): break;
          case CSL_TYPE_SEL(end_of_record): break;

          case CSL_TYPE_SEL(return_from_function):
            conv_data(res,"<RET>");
            break;

          case CSL_TYPE_SEL(const char *):
            {
              uint16_t slen = 0; get_data<uint16_t>(encoded,len,slen,pos);
              if( slen > 0 )
              {
                char * p = stmp.allocate_nocopy(slen);
                if( p != NULL )
                {
                  get_data(encoded,len,p,stmp.nbytes(),pos);
                  stmp.append('\0');
                  /*conv_data(res,L"\n  String: "); conv_data(res,stmp.data(),slen);*/
                  conv_data(res2,stmp.data(),slen);

                }
              }
            }
            break;

          case CSL_TYPE_SEL(const wchar_t *):
            {
              uint16_t slen = 0; get_data<uint16_t>(encoded,len,slen,pos);
              if( slen > 0 )
              {
                wchar_t * p = wstmp.allocate_nocopy(slen);
                if( p != NULL )
                {
                  get_data(encoded,len,p,wstmp.nbytes(),pos);
                  wstmp.append(L'\0');
                  /*conv_data(res,L"\n  WString: "); conv_data(res,wstmp.data(),slen);*/
                  conv_data(res2,wstmp.data(),slen);
                }
              }
            }
            break;

          case CSL_TYPE_SEL(when): break;
          case CSL_TYPE_SEL(threadid): break;
          case CSL_TYPE_SEL(procid): break;
          case CSL_TYPE_SEL(hostid): break;
          case CSL_TYPE_SEL(seqno): break;

          case CSL_TYPE_SEL(location_id):
            get_data<uint32_t>(encoded,len,locid,pos);
            conv_locid(res,locid);
            break;

          default: break;
          };
        }
        if( res2.size() > 0 )
        {
          res2.append(L'\0');
          conv_data(res,"@: "); conv_data(res,res2.data(),res2.size()-1);
        }
        //conv_end(res);
        conv_data(res,L"\n");
        result.assign(res.data(),res.data()+res.size());
      }
    }
  }
}
