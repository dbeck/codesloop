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

#ifndef _csl_common_stream_hh_included_
#define _csl_common_stream_hh_included_
#include "codesloop/common/stream_part.hh"
#include "codesloop/common/common.h"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class stream
    {
    public:      
      // packet frame
      virtual void start() = 0;
      virtual void end()   = 0;
      virtual void flush() = 0;
      
      // output interface
      virtual stream_part reserve(size_t sz) = 0;
      virtual void confirm(const stream_part & p, size_t sz) = 0;
      virtual void push(const stream_part & p) = 0;
      
      // input interface
      virtual stream_part get(size_t sz) = 0;
      virtual size_t poll(size_t sz, uint32_t & timeout_ms) = 0;
      
      // stats
      virtual size_t position() const = 0;
      virtual size_t available() const = 0;
      
      // event interface
      class event
      {
      public:
        virtual void operator()(int which) {}
        virtual ~event() {}
      };
      
      static const int start_event_ = 0;
      static const int end_event_   = 1;
      static const int flush_event_ = 2;
      static const int data_event_  = 3;
      static const int empty_event_ = 4;
      
      virtual void event_cb(event & ev, int which) = 0;

      virtual ~stream() {}
    };
  }
}

#endif /*__cplusplus*/
#endif /*_csl_common_stream_hh_included_*/
