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

#ifndef _csl_common_stream_part_hh_included_
#define _csl_common_stream_part_hh_included_
#include "codesloop/common/common.h"
#ifdef __cplusplus
namespace csl
{
  namespace common
  {
    class stream_part
    {
    public:
      typedef uint8_t item_t;
      
      item_t * data() const  { return data_;  }
      size_t   items() const { return items_; }
      
      void data(item_t * d)  { data_  = d; }
      void items(size_t s)   { items_ = s; }
      
      stream_part() : data_(0), items_(0) {}
      stream_part(item_t * d, size_t s) : data_(d), items_(s) {}
      
      void reset() { data_ = 0; items_ = 0; }
      
    private:
      item_t * data_;
      size_t   items_;      
    };
  }
}
    
#endif /*__cplusplus*/
#endif /*_csl_common_stream_part_hh_included_*/

