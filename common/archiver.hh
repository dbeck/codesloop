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

#ifndef _csl_common_archiver_hh_included_
#define _csl_common_archiver_hh_included_
#include "codesloop/common/common.h"
#include "codesloop/common/stream.hh"
#include "codesloop/common/obj.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class archiver
    {
      public:
        enum direction { SERIALIZE, DESERIALIZE };
        
        archiver( direction d, stream_base & s, uint32_t timeout_ms = 50 );
        virtual ~archiver() {}

        /* abstract interface */
        virtual void reset() = 0;

        virtual archiver & serialize( str & v ) = 0;
        virtual archiver & serialize( ustr & v ) = 0;
        virtual archiver & serialize( dbl & v ) = 0;
        virtual archiver & serialize( int64 & v ) = 0;
        virtual archiver & serialize( binry & v ) = 0;

        virtual archiver & serialize( int8_t & v ) = 0;
        virtual archiver & serialize( int16_t & v ) = 0;
        virtual archiver & serialize( int32_t & v ) = 0;
        virtual archiver & serialize( int64_t & v ) = 0;
        
        virtual archiver & serialize( uint8_t & v ) = 0;
        virtual archiver & serialize( uint16_t & v ) = 0;
        virtual archiver & serialize( uint32_t & v ) = 0;        
        virtual archiver & serialize( uint64_t & v ) = 0;

        virtual archiver & serialize( float & v ) = 0;
        virtual archiver & serialize( double & v ) = 0;
        
        /* accessors */
        direction get_direction() const               { return dir_;      }
        const stream_base & get_const_stream() const  { return *stream_;  }
        uint32_t get_timeout() const                  { return timeout_;  }
        stream_base & get_stream()                    { return *stream_;  }
        
        /* manipulators */
        void set_direction( direction d )        { dir_ = d; }
        void set_stream( stream_base & s )       { stream_ = &s; }
        void set_timeout( uint32_t timeout_ms )  { timeout_ = timeout_ms; }
        
      private:
        direction      dir_;
        stream_base *  stream_;
        uint32_t       timeout_;
        
        /* no default construction */
        archiver() : dir_(SERIALIZE), stream_(NULL), timeout_(0) {}
        
        /* no copy */
        archiver(const archiver &) : dir_(SERIALIZE), stream_(NULL), timeout_(0) {}
        archiver & operator=(const archiver &) { return *this; }
    };
  }
}

#endif /*__cplusplus*/
#endif /*_csl_common_archiver_hh_included_*/

