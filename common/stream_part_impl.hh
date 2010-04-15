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

#if 0
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
//#define DEBUG_VERBOSE
#endif /* DEBUG */
#endif /* 0 */

#include "codesloop/common/logger.hh"
#include "codesloop/common/str.hh"

namespace csl
{
  namespace common
  {
    template<typename T>
    stream_part<T>::stream_part() : data_(0), bytes_(0) { }

    template<typename T>
    void stream_part<T>::reset()
    {
      ENTER_FUNCTION();
      data_       = 0;
      bytes_      = 0;
      flags_.set_flags(stream_flags::ok_);
      LEAVE_FUNCTION();
    }

    template<typename T>
    T * stream_part<T>::data() const
    {
      ENTER_FUNCTION();
      RETURN_FUNCTION( data_ );
    }

    template<typename T>
    void stream_part<T>::data(T * d)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"data(%p)",d );
      data_ = d;
      LEAVE_FUNCTION();
    }

    template<typename T>
    size_t stream_part<T>::bytes() const
    {
      ENTER_FUNCTION();
      RETURN_FUNCTION( bytes_ );
    }

    template<typename T>
    void stream_part<T>::bytes(size_t b)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"bytes(%lld)",static_cast<uint64_t>(b) );
      bytes_ = b;
      LEAVE_FUNCTION();
    }

    template<typename T>
    stream_flags & stream_part<T>::flags() 
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str;
      flags_.to_str(flags_str);
      CSL_DEBUGF(L"flags() => [%x:%ls]",
        flags_.flags(),flags_str.c_str());
#endif /*DEBUG*/
      RETURN_FUNCTION( flags_ );
    }

  } /* end of ns: csl::common */
} /* end of ns: csl */

/* EOF */
