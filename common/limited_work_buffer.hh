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

#ifndef _csl_common_limited_work_buffer_hh_included_
#define _csl_common_limited_work_buffer_hh_included_

#include "codesloop/common/stream_part.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/tbuf.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/logger.hh"
#include "codesloop/common/obj.hh"

#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <size_t Preallocated=1024,size_t MaxSize=256*1024>
    class limited_work_buffer
    {
      public:
        static const size_t preallocated_size_ = Preallocated;
        static const size_t max_size_          = MaxSize;

        stream_part & get( size_t sz, stream_part & sp )
        {
          ENTER_FUNCTION();
          CSL_DEBUGF(L"get(sz:%lld,sp)",static_cast<uint64_t>(sz));
          sp.reset();

          if( sz == 0 )
          {
            CSL_DEBUGF(L"invalid size [sz:0]");
            sp.failed( true );
          }
          else if( len_ > 0 )
          {
            size_t ret_size = (len_ > sz ? sz : len_);
            sp.data( buf_.private_data() + start_ );
            sp.bytes( ret_size );
            start_ += ret_size;
            len_   -= ret_size;
            if( len_ == 0 ) start_ = 0;
          }
          else
          {
            // this is not an error: empty buffer
            CSL_DEBUGF(L"no data to be returned");
          }
          RETURN_FUNCTION( sp );
        }

        // there is an important design decision here: data is always allocated at
        // the end of the buffer. even if space is available at the beginning of
        // the buffer, it can only be used if all data is returned by get().
        // this behaviour forces the application to care about the buffer, thus
        // not not enforcing unneccessary and time consuming memory copies.
        stream_part & reserve( size_t sz, stream_part & sp )
        {
          ENTER_FUNCTION();
          CSL_DEBUGF(L"resrerve(sz:%lld,sp)",static_cast<uint64_t>(sz));
          sp.reset();

          size_t new_len = start_ + len_ + sz;
          if( new_len > MaxSize )
          {
            CSL_DEBUGF(L"cannot allocate %lld bytes",static_cast<uint64_t>(sz));
            if( start_ + len_ < MaxSize )
            {
              CSL_DEBUGF(L"allocate additional %lld bytes instead of the "
                         "requested %lld bytes [max:%lld-start_:%lld-len_:%lld]",
                         static_cast<uint64_t>(n_free()),
                         static_cast<uint64_t>(sz),
                         static_cast<uint64_t>(start_),
                         static_cast<uint64_t>(len_));
              uint8_t * ptr = buf_.allocate( MaxSize );
              sp.data( ptr + start_ + len_ );
              sp.bytes( n_free() );
              len_ = MaxSize - start_;
            }
            else
            {
              // we are at maximum capacity already: this is an error
              CSL_DEBUGF(L"cannot allocate more data");
              sp.failed( true );
            }
          }
          else if( sz == 0 )
          {
            CSL_DEBUGF(L"not allocating");
            sp.reset(); // this is to enforce errors
          }
          else
          {
            CSL_DEBUGF(L"allocating %lld bytes",static_cast<uint64_t>(sz));
            uint8_t * ptr = buf_.allocate( len_+sz );
            sp.data( ptr + start_ + len_ );
            sp.bytes( sz );
            len_ += sz;
          }
          RETURN_FUNCTION( sp );
        }

        stream_part & adjust( stream_part & sp, size_t n_succeed )
        {
          ENTER_FUNCTION();
          CSL_DEBUGF(L"adjust(sp,n_succeed:%lld)",static_cast<uint64_t>(n_succeed));

          size_t start_offset = 0;
          size_t adjust_len   = 0;

          start_offset = sp.data() - buf_.private_data();

          if( sp.bytes() < n_succeed ||
              sp.data() == NULL      ||
              sp.failed() == true    ||
              len_ < sp.bytes()      ||
              start_offset !=  (start_+len_-sp.bytes()) )
          {
            CSL_DEBUGF(L"invalid param received");
            goto bail;
          }

          {
            // set sp 
            adjust_len = sp.bytes() - n_succeed;
            sp.bytes( n_succeed );
            if( !n_succeed ) sp.data( NULL );
          }

          {
            // set internal data
            len_ -= adjust_len;
            if( len_ == 0 )           { start_ = 0; buf_.allocate(0); }
            else if( adjust_len > 0 )
            {
              uint8_t * p = buf_.allocate( len_ + start_ );
              if( n_succeed > 0 ) { sp.data( p + start_offset ); }
            }
          }

          CSL_DEBUGF(L"length decreased by: %lld bytes",static_cast<uint64_t>(adjust_len));
        bail:
          RETURN_FUNCTION( sp );
        }

        size_t start()  const { return start_;      }
        size_t len()    const { return len_;        }
        size_t buflen() const { return buf_.size(); }
        size_t n_free() const { return (MaxSize-len_-start_); }

        const unsigned char * data() const { return buf_.data()+start_; }

        limited_work_buffer() : start_(0), len_(0), use_exc_(true) {}

      private:
        common::tbuf<Preallocated> buf_;
        size_t start_;
        size_t len_;

        CSL_OBJ(csl::common,limited_work_buffer);
        USE_EXC();
    };
  } /* end of ns:csl:common */
} /* end of ns:csl */

#endif /* __cplusplus */
#endif /* _csl_common_limited_work_buffer_hh_included_ */
