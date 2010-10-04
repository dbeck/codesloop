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
#include "codesloop/common/obj.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class str;
    class ustr;
    class dbl;
    class int64;
    class binry;
    class pbuf;
    class stream_base;
    
    class archiver
    {
      public:
        enum direction { SERIALIZE, DESERIALIZE };
        
        archiver( direction d, stream_base & s, uint32_t timeout_ms = 50 )
          : dir_(d), stream_(&s), timeout_(timeout_ms) {}

        virtual ~archiver() {}

        /* combined interface for push and pop */
        template <typename T>
        archiver & serialize( T & v )
        {
          if( get_direction() == SERIALIZE )
          {
            push( get_stream(), v );
          }
          else if( get_direction() == DESERIALIZE )
          {
            pop( get_stream(), v, timeout_ref() );
          }
          return *this;
        }

        /* abstract interface */
        virtual void reset() = 0;
                                
        /* separate interface for push */
        virtual archiver & push( stream_base & b, const str & v ) = 0;
        virtual archiver & push( stream_base & b, const ustr & v ) = 0;
        virtual archiver & push( stream_base & b, const dbl & v ) = 0;
        virtual archiver & push( stream_base & b, const int64 & v ) = 0;
        virtual archiver & push( stream_base & b, const binry & v ) = 0;
        virtual archiver & push( stream_base & b, const pbuf & v ) = 0;

        virtual archiver & push( stream_base & b, const int8_t & v ) = 0;
        virtual archiver & push( stream_base & b, const int16_t & v ) = 0;
        virtual archiver & push( stream_base & b, const int32_t & v ) = 0;
        virtual archiver & push( stream_base & b, const int64_t & v ) = 0;
        
        virtual archiver & push( stream_base & b, const uint8_t & v ) = 0;
        virtual archiver & push( stream_base & b, const uint16_t & v ) = 0;
        virtual archiver & push( stream_base & b, const uint32_t & v ) = 0;        
        virtual archiver & push( stream_base & b, const uint64_t & v ) = 0;

        virtual archiver & push( stream_base & b, const float & v ) = 0;
        virtual archiver & push( stream_base & b, const double & v ) = 0;
                
        /* separate interface for pop */
        virtual archiver & pop( stream_base & b, str & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, ustr & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, dbl & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, int64 & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, binry & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, pbuf & v, uint32_t & timeout_ms ) = 0;

        virtual archiver & pop( stream_base & b, int8_t & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, int16_t & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, int32_t & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, int64_t & v, uint32_t & timeout_ms ) = 0;
        
        virtual archiver & pop( stream_base & b, uint8_t & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, uint16_t & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, uint32_t & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, uint64_t & v, uint32_t & timeout_ms ) = 0;

        virtual archiver & pop( stream_base & b, float & v, uint32_t & timeout_ms ) = 0;
        virtual archiver & pop( stream_base & b, double & v, uint32_t & timeout_ms ) = 0;
                        
        /* accessors */
        direction get_direction() const               { return dir_;      }
        const stream_base & get_const_stream() const  { return *stream_;  }
        uint32_t get_timeout() const                  { return timeout_;  }
        stream_base & get_stream()                    { return *stream_;  }
        
        /* manipulators */
        void set_direction( direction d )        { dir_ = d; }
        void set_stream( stream_base & s )       { stream_ = &s; }
        void set_timeout( uint32_t timeout_ms )  { timeout_ = timeout_ms; }
        
      protected:
        uint32_t & timeout_ref() { return timeout_; }
        
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

