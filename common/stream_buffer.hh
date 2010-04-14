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

#ifndef _csl_common_stream_buffer_hh_included_
#define _csl_common_stream_buffer_hh_included_

#include "codesloop/common/stream_part.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/preallocated_array.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/logger.hh"
#include "codesloop/common/obj.hh"

#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <typename T, size_t Preallocated=1024, size_t MaxSize=256*1024>
    class stream_buffer
    {
      public:
        static const size_t preallocated_size_ = Preallocated;
        static const size_t max_size_          = MaxSize;
        typedef T item_t;
        typedef stream_part<T> part_t;

        part_t & get( size_t sz, part_t & sp );

        // there is an important design decision here: data is always allocated at
        // the end of the buffer. even if space is available at the beginning of
        // the buffer, it can only be used if all data is returned by get().
        // this behaviour forces the application to care about the buffer, thus
        // not enforcing unneccessary and time consuming memory copies.
        
        part_t & reserve( size_t sz, part_t & sp );        
        part_t & confirm( size_t n_succeed, part_t & sp );
        
        size_t start()  const { return start_;      }
        size_t len()    const { return len_;        }
        size_t buflen() const { return buf_.size(); }
        size_t n_free() const { return (MaxSize-len_-start_); }

        const unsigned char * data() const { return buf_.data()+start_; }

        stream_buffer() : start_(0), len_(0), use_exc_(true) {}

      private:
        preallocated_array<T,Preallocated> buf_;
        size_t start_;
        size_t len_;

        CSL_OBJ(csl::common,stream_buffer);
        USE_EXC();
    };
  } /* end of ns:csl:common */
} /* end of ns:csl */

#endif /* __cplusplus */
#include "codesloop/common/stream_buffer_impl.hh"
#endif /* _csl_common_stream_buffer_hh_included_ */

