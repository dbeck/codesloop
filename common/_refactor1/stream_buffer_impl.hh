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

#ifndef _csl_common_stream_buffer_impl_hh_included_
#define _csl_common_stream_buffer_impl_hh_included_
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <typename T, size_t Preallocated, size_t MaxSize>
    typename stream_buffer<T,Preallocated,MaxSize>::part_t &
    stream_buffer<T,Preallocated,MaxSize>::get( size_t sz,
                                                stream_buffer<T,Preallocated,MaxSize>::part_t & sp )
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"get(sz:%lld,sp)",static_cast<uint64_t>(sz));
      sp.reset();

      if( sz == 0 )
      {
        CSL_DEBUGF(L"invalid size [sz:0]");
        sp.flags() << stream_flags::parameter_error_;
      }
      else if( len_ > 0 )
      {
        size_t ret_size = (len_ > sz ? sz : len_);
        sp.data( buf_.private_data() + start_ );
        sp.items( ret_size );
        start_ += ret_size;
        len_   -= ret_size;
        if( len_ == 0 ) start_ = 0;
      }
      else
      {
        // this is not an error: empty buffer
        sp.flags() << stream_flags::empty_buffer_;
        CSL_DEBUGF(L"no data to be returned");
      }
      RETURN_FUNCTION( sp );
    }

    template <typename T, size_t Preallocated, size_t MaxSize>  
    typename stream_buffer<T,Preallocated,MaxSize>::part_t &
    stream_buffer<T,Preallocated,MaxSize>::reserve( size_t sz,
                                                    stream_buffer<T,Preallocated,MaxSize>::part_t & sp )
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"resrerve(sz:%lld,sp)",static_cast<uint64_t>(sz));
      sp.reset();

      size_t new_len = start_ + len_ + sz;
      if( new_len > MaxSize )
      {
        CSL_DEBUGF(L"cannot allocate %lld items",static_cast<uint64_t>(sz));
        if( start_ + len_ < MaxSize )
        {
          CSL_DEBUGF(L"allocate additional %lld items instead of the "
                     "requested %lld items [max:%lld-start_:%lld-len_:%lld]",
                     static_cast<uint64_t>(n_free()),
                     static_cast<uint64_t>(sz),
                     static_cast<uint64_t>(start_),
                     static_cast<uint64_t>(len_));

          T * ptr = buf_.allocate( MaxSize );
          sp.data( ptr + start_ + len_ );
          sp.items( n_free() );
          len_ = MaxSize - start_;
          sp.flags() << stream_flags::partially_allocated_;
        }
        else
        {
          // we are at maximum capacity already: this is an error
          CSL_DEBUGF(L"cannot allocate more data");
          sp.flags() << stream_flags::buffer_full_;
        }
      }
      else if( sz == 0 )
      {
        CSL_DEBUGF(L"not allocating");
        sp.reset(); // this is to enforce errors
        sp.flags() << stream_flags::parameter_error_;
      }
      else
      {
        CSL_DEBUGF(L"allocating %lld items",static_cast<uint64_t>(sz));
        T * ptr = buf_.allocate( len_+sz );
        sp.data( ptr + start_ + len_ );
        sp.items( sz );
        len_ += sz;
      }
      RETURN_FUNCTION( sp );
    }

    template <typename T, size_t Preallocated, size_t MaxSize>
    typename stream_buffer<T,Preallocated,MaxSize>::part_t &
    stream_buffer<T,Preallocated,MaxSize>::confirm( size_t n_succeed,
                                                    stream_buffer<T,Preallocated,MaxSize>::part_t & sp )
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"confirm(sp,n_succeed:%lld)",static_cast<uint64_t>(n_succeed));

      size_t start_offset  = 0;
      size_t confirm_len   = 0;

      start_offset = sp.data() - buf_.private_data();

      if( sp.items() < n_succeed            ||
          sp.data() == NULL                 ||
          len_ < sp.items()                 ||
          start_offset !=  (start_+len_-sp.items()) )
      {
        CSL_DEBUGF(L"invalid param received");
        sp.flags() << stream_flags::parameter_error_;
        goto bail;
      }

      {
        // set sp 
        confirm_len = sp.items() - n_succeed;
        sp.items( n_succeed );
        if( !n_succeed ) sp.data( NULL );
      }

      {
        // set internal data
        len_ -= confirm_len;
        if( len_ == 0 ) { start_ = 0; buf_.allocate(0); }
        else if( confirm_len > 0 )
        {
          T * p = buf_.allocate( len_ + start_ );

          // if allocate modifies the pointer, then it should be
          // reflected in the stream part too
          if( n_succeed > 0 ) { sp.data( p + start_offset ); }
        }
      }

      CSL_DEBUGF(L"length decreased by: %lld items",static_cast<uint64_t>(confirm_len));
    bail:
      RETURN_FUNCTION( sp );
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_stream_buffer_impl_hh_included_ */

