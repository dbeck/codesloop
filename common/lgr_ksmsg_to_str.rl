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

#include "codesloop/common/ksmsg.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/stpodary.hh"
#include "codesloop/common/strconcat.hh"
#include "codesloop/common/trailing_zero.hh"
#include "codesloop/common/lgr_msg.hh"

namespace
{
  using csl::common::stpodary;
  using csl::common::strconcat;
  using csl::common::trailing_zero;
  using namespace csl::common::lgr;
  
  static const unsigned char tv = type_select<tag>::sel_; 
  
  %%{
    machine ksmsg2str;
    alphtype unsigned char;

    main := ( 'foo' );
  }%%
  
  %% write data;
  
  typedef csl::common::stpodary<wchar_t,2048> result_t;
    
  void strip_trailing_zero( result_t & res )
  {
    if( res.size() > 0 && trailing_zero<result_t>::check(res) )
    {
      res.allocate( res.size()-1 );
    }  
  }
  
  void add_data( result_t & res, bool val )
  {
    res.append( L" b:", 3  );
    if( val ) res.append( L"true", 4 );
    else      res.append( L"false", 5 ); 
  }
  
  void add_data( result_t & res, int64_t val )
  {
    wchar_t tmp[28];
    int len = swprintf(tmp,27, L" i:%lld",val );
    if( len != -1 ) res.append( tmp,len );
  }
  
  void add_data( result_t & res, double val )
  {
    wchar_t tmp[28];
    int len = swprintf(tmp,27, L" f:%.6f",val );
    if( len != -1 ) res.append( tmp,len );
  }
  
  void add_data( result_t & res, const char * s, size_t len )
  {
    trailing_zero<result_t>::ensure(res);
    strconcat<result_t>::execute(res,s,len);
    strip_trailing_zero( res );
  }

  void add_data( result_t & res, const wchar_t * s, size_t len )
  {
    trailing_zero<result_t>::ensure(res);
    strconcat<result_t>::execute(res,s,len);
    strip_trailing_zero( res );
  }

}

namespace csl
{
  namespace common
  {
    namespace lgr
    {
      void to_str(ksmsg m, str & s)
      {
        scoped_kspin_lock lck(m.get_lock());
        int cs;
        const uint8_t * p  = m.buffer().buf_;
        const uint8_t * pe = p+(m.buffer().len_);
        %% write init;
        %% write exec;
      }
    }
  }
}

