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

#ifndef _csl_common_lgr_msg_hh_types_included_
#define _csl_common_lgr_msg_hh_types_included_

#include "codesloop/common/common.h"

namespace csl
{
  namespace common
  {
    namespace lgr // logger helpers
    {
      struct tag {};
      struct name {};
      struct end_of_record {};
      struct return_from_function {};
      struct when {};
      struct threadid {};
      struct procid {};
      struct hostid {};
      struct seqno {};
      struct location_id {};
      struct max_value {};
      struct disabled_log {};

      template <typename T> struct type_select { };

#ifndef CSL_DECLARE_LOG_TYPE_SELECT
#define CSL_DECLARE_LOG_TYPE_SELECT(TYPE, ID)                   \
    template <> struct type_select<TYPE>                        \
    {                                                           \
      enum { val_ =  ID };                                      \
      static const uint8_t sel_;                                \
      typedef TYPE type_t;                                      \
      static const char * as_str() { return #TYPE; };           \
      static const wchar_t * as_wstr() { return L""#TYPE; };    \
    }
#endif // CSL_DECLARE_LOG_TYPE_SELECT

      CSL_DECLARE_LOG_TYPE_SELECT( bool,                  1  );
      CSL_DECLARE_LOG_TYPE_SELECT( int8_t,                2  );
      CSL_DECLARE_LOG_TYPE_SELECT( int16_t,               3  );
      CSL_DECLARE_LOG_TYPE_SELECT( int32_t,               4  );
      CSL_DECLARE_LOG_TYPE_SELECT( int64_t,               5  );
      CSL_DECLARE_LOG_TYPE_SELECT( uint8_t,               6  );
      CSL_DECLARE_LOG_TYPE_SELECT( uint16_t,              7  );
      CSL_DECLARE_LOG_TYPE_SELECT( uint32_t,              8  );
      CSL_DECLARE_LOG_TYPE_SELECT( uint64_t,              9  );
      CSL_DECLARE_LOG_TYPE_SELECT( float,                 10 );
      CSL_DECLARE_LOG_TYPE_SELECT( double,                11 );
      CSL_DECLARE_LOG_TYPE_SELECT( tag,                   12 );
      CSL_DECLARE_LOG_TYPE_SELECT( name,                  13 );
      CSL_DECLARE_LOG_TYPE_SELECT( end_of_record,         14 );
      CSL_DECLARE_LOG_TYPE_SELECT( return_from_function,  15 );
      CSL_DECLARE_LOG_TYPE_SELECT( when,                  16 );
      CSL_DECLARE_LOG_TYPE_SELECT( threadid,              17 );
      CSL_DECLARE_LOG_TYPE_SELECT( procid,                18 );
      CSL_DECLARE_LOG_TYPE_SELECT( hostid,                19 );
      CSL_DECLARE_LOG_TYPE_SELECT( seqno,                 20 );
      CSL_DECLARE_LOG_TYPE_SELECT( location_id,           21 );
      CSL_DECLARE_LOG_TYPE_SELECT( const char *,          22 );
      CSL_DECLARE_LOG_TYPE_SELECT( const wchar_t *,       23 );
      // keep this up to date:
      CSL_DECLARE_LOG_TYPE_SELECT( max_value,             23 );
    }
  }
}

#endif // _csl_common_lgr_msg_hh_types_included_
