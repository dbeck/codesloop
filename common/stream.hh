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
    template <typename T>
    class stream_base
    {
    public:
      typedef stream_part<T> part_t;
      
      /* packet frame */
      virtual const stream_flags & start() = 0;
      virtual const stream_flags & end() = 0;
      virtual const stream_flags & flush() = 0;
      
      /* error handling */
      virtual stream_flags & flags() = 0;

      /* buffer allocation */
      virtual part_t & reserve( size_t sz, part_t & sp ) = 0;
      virtual part_t & confirm( size_t n_succeed, part_t & sp ) = 0;
      virtual part_t & get( size_t sz, part_t & sp ) = 0;
      
      /* sizes */
      virtual size_t confirmed_items() = 0;
      
      virtual ~stream_base() {}
    };
    
    // Target is where the streamed data should arrive. This is an asynch interface where
    // target is notified by specific notification calls from the stream. The stream client
    // is expected to call this sequence:
    //
    //  -- start()    -- tells the stream that a new stream packet is assembling
    //  -- reserve()  -- allocate the needed space from the buffer
    //     ....       -- put the data into the allocated buffer
    //  -- confirm()  -- tell the stream, that we are done in the buffer. the stream
    //                   calls the target's data() functions, so it can get() the assembled
    //                   data
    //  -- end()      -- tell the stream that we are done with this packet

    // Target must provide the following interface:
    //
    //  -- these are used in the stream to tell the target that a new packet has started
    //     ended, data arrived ...
    //
    //    const stream_flags & start(stream_base &);          -- to tell that a new stream packet is arriving
    //    const stream_flags & end(stream_base &);            -- to tell that the packet has been finished
    //    const stream_flags & flush(stream_base &);          -- to tell that the data stream reached a logical
    //                                                           boundary. eg. a video frame in a video stream
    //    const stream_flags & data(stream_base &, size_t);   -- to tell that data arrived to the stream
    
    // The stream call sequence is this (C=client, S=stream, B=buffer, T=target)
    //
    //   C -> S: start    -> T: start               -- get ready for a new packet
    //   C -> S: reserve  -> B: reserve             -- allocate space for the data
    //   C (assemble data in the buffer allocated)
    //   C -> S: confirm  -> {B: confirm, T: data}  -- buffer adjusts its buffer boundaries
    //                                                 based on the  confirmed data size.
    //                                                 target is notified about data arrival
    //   B: get <- S: get <- T                      -- target gets data from the stream (buffer)
    //   C -> S: end      -> T: end                 -- client declares that no more data to be sent
    //                                                 so target can do appropriate actions (eg. add
    //                                                 cryptographic headers, close socket, etc...)
    
    template <typename T,
              template <typename,size_t,size_t> class Buffer = stream_buffer,
              template <typename> class Target = stream_nop_target,
              size_t Preallocated=1024,
              size_t MaxSize=(256*1024)>
    class stream : public stream_base<T>
    {
    public:
      typedef stream_base<T>                  base_t;
      typedef stream_part<T>                  part_t;
      typedef Target<T>                       target_t;
      typedef Buffer<T,Preallocated,MaxSize>  buffer_t;
      
      /* packet frame */
      const stream_flags & start();
      const stream_flags & end();
      const stream_flags & flush();
      
      /* error handling */
      stream_flags & flags();

      /* buffer allocation */
      part_t & reserve( size_t sz, part_t & sp );
      part_t & confirm( size_t n_succeed, part_t & sp );
      part_t & get( size_t sz, part_t & sp );
      
      /* sizes */
      size_t confirmed_items();
      
      virtual ~stream() {}
      stream(target_t & t, buffer_t & b);
      stream(buffer_t & b);
      void set_target(target_t & t);
      
    private:
      // no default construction: at least a buffer is needed
      stream() : target_(0), buffer_(0), n_confirmed_(0) {}
      
      stream_flags  flags_;
      target_t *    target_;
      buffer_t *    buffer_;
      size_t        n_confirmed_;
      
      CSL_OBJ(csl::common,stream);
    };
    
  } /* end of ns:csl:common */
} /* end of ns:csl */

#endif /* __cplusplus */
#include "codesloop/common/stream_impl.hh"
#endif /* _csl_common_stream_hh_included_ */

