/*
Copyright (c) 2008,2009,2010, CodeSLoop Team

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

#ifndef _csl_common_ydr_archiver_hh_included_
#define _csl_common_ydr_archiver_hh_included_
#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#include "codesloop/common/archiver.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class ydr_archiver : public csl::common::archiver
    {
      public:
        ydr_archiver( direction d, stream_base & s, uint32_t timeout_ms = 50 );
        virtual ~ydr_archiver() {}

        /* abstract interface */
        void reset();

        archiver & serialize( str & v );
        archiver & serialize( ustr & v );
        archiver & serialize( dbl & v ) ;
        archiver & serialize( int64 & v );
        archiver & serialize( binry & v );

        archiver & serialize( int8_t & v );
        archiver & serialize( int16_t & v );
        archiver & serialize( int32_t & v );
        archiver & serialize( int64_t & v );
        
        archiver & serialize( uint8_t & v );
        archiver & serialize( uint16_t & v );
        archiver & serialize( uint32_t & v );        
        archiver & serialize( uint64_t & v );

        archiver & serialize( float & v );
        archiver & serialize( double & v );

      private:
        /* no default construction */
        ydr_archiver() : archiver(SERIALIZE, *(reinterpret_cast<stream_base*>(NULL))) {}
        
        /* no copy */
        ydr_archiver(const archiver &) : archiver(SERIALIZE, *(reinterpret_cast<stream_base*>(NULL))) {}
        ydr_archiver & operator=(const archiver &) { return *this; }

    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_ydr_archiver_hh_included_ */

