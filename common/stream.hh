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

#ifndef _csl_common_stream_hh_included_
#define _csl_common_stream_hh_included_

#include "codesloop/common/stream_part.hh"
#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream_flags.hh"
#include "codesloop/common/stream_nop_target.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/logger.hh"
#include "codesloop/common/obj.hh"

#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class stream_base
    {
    public:
      typedef uint8_t             item_t;
      typedef stream_part<item_t> part_t;

      /* packet frame */
      virtual const stream_flags & start() = 0;
      virtual const stream_flags & end() = 0;
      virtual const stream_flags & flush() = 0;

      /* error handling */
      virtual stream_flags & flags() = 0;

      /* buffer allocation */
      virtual part_t & reserve( size_t sz, part_t & sp ) = 0;
      virtual part_t & confirm( size_t n_succeed, part_t & sp ) = 0;

      /* receiving data */
      virtual part_t & get( size_t sz, part_t & sp ) = 0;
      virtual const stream_flags & poll( size_t requested_items,
                                         size_t & available_items,
                                         uint32_t & timeout_ms ) = 0;

      /* sizes */
      virtual size_t total_confirmed_items() = 0;
      virtual size_t free_item_count() = 0;
      virtual size_t max_item_count() = 0;
      virtual size_t has_n_items() = 0;

      virtual ~stream_base() {}
    };

    template <template <typename,size_t,size_t> class Buffer = stream_buffer,
              size_t Preallocated=1024,
              size_t MaxSize=(256*1024)>
    class buffered_stream : public stream_base
    {
    public:
      typedef Buffer<item_t,Preallocated,MaxSize> buffer_t;

      /* packet frame */
      virtual const stream_flags & start();
      virtual const stream_flags & end();
      virtual const stream_flags & flush();

      /* error handling */
      stream_flags & flags();

      /* buffer allocation */
      part_t & reserve( size_t sz, part_t & sp );
      part_t & confirm( size_t n_succeed, part_t & sp );

      /* receiving data */
      part_t & get( size_t sz, part_t & sp );
      virtual const stream_flags & poll( size_t requested_items,
                                         size_t & available_items,
                                         uint32_t & timeout_ms );

      /* sizes */
      size_t total_confirmed_items();
      size_t free_item_count();
      size_t max_item_count();
      size_t has_n_items();

      virtual ~buffered_stream() {}
      buffered_stream(buffer_t & b);

    protected:
      /* event upcalls */
      virtual void on_reserved( size_t sz ) {}
      virtual void on_confirmed( size_t n_succeed ) {}
      virtual void on_get( size_t sz ) {}
      virtual void on_poll( size_t requested_items,
                            size_t & available_items,
                            uint32_t & timeout_ms ) {}

    private:
      // no default construction: at least a buffer is needed
      buffered_stream() : buffer_(0), n_confirmed_(0) {}

      stream_flags  flags_;
      buffer_t *    buffer_;
      size_t        n_confirmed_;

      CSL_OBJ(csl::common,buffered_stream);
    };

    template <class Target = stream_nop_target,
              template <typename,size_t,size_t> class Buffer = stream_buffer,
              size_t Preallocated=1024,
              size_t MaxSize=(256*1024)>
    class output_stream : public buffered_stream<Buffer,Preallocated,MaxSize>
    {
    public:
      typedef Target                                            target_t;
      typedef Buffer<stream_base::item_t,Preallocated,MaxSize>  buffer_t;
      typedef buffered_stream<Buffer,Preallocated,MaxSize>      buffered_stream_t;

      /* packet frame */
      const stream_flags & start();
      const stream_flags & end();
      const stream_flags & flush();

      /* error handling */
      stream_flags & flags();

      virtual ~output_stream() {}
      output_stream(target_t & t, buffer_t & b);
      output_stream(buffer_t & b);
      void set_target(target_t & t);

    private:
      // no default construction
      output_stream()
        : buffered_stream<Buffer,Preallocated,MaxSize>(*(new buffer_t())),
          target_(0) { throw "not implemented"; }

      stream_flags  flags_;
      target_t *    target_;

      CSL_OBJ(csl::common,output_stream);
    };

    template <class Source,
              template <typename,size_t,size_t> class Buffer = stream_buffer,
              size_t Preallocated=1024,
              size_t MaxSize=(256*1024)>
    class input_stream : public buffered_stream<Buffer,Preallocated,MaxSize>
    {
    public:
      typedef Source                                           source_t;
      typedef Buffer<stream_base::item_t,Preallocated,MaxSize> buffer_t;
      typedef buffered_stream<Buffer,Preallocated,MaxSize>     buffered_stream_t;

      /* packet frame */
      const stream_flags & start();
      const stream_flags & end();
      const stream_flags & flush();

      /* receiving data */
      const stream_flags & poll( size_t requested_items,
                                 size_t & available_items,
                                 uint32_t & timeout_ms );

      /* error handling */
      stream_flags & flags();

      virtual ~input_stream() {}
      input_stream(source_t & s, buffer_t & b);

    private:
      // no default construction
      input_stream()
        : buffered_stream<Buffer,Preallocated,MaxSize>(*(new buffer_t())),
          source_(0) { throw "not implemented"; }

      stream_flags  flags_;
      source_t *    source_;

      CSL_OBJ(csl::common,input_stream);
    };

  } /* end of ns:csl:common */
} /* end of ns:csl */

#endif /* __cplusplus */
#include "codesloop/common/stream_impl.hh"
#endif /* _csl_common_stream_hh_included_ */
