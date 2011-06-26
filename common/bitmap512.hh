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

#ifndef _csl_common_bitmap512_hh_included_
#define _csl_common_bitmap512_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/dbc.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class bitmap512
    {
    private:
      static uint8_t empty_[64];
      static uint8_t first_free_[256];
      static uint8_t max_free_[256];
      static uint8_t max_pos_[256];
      static uint8_t max_posB_[256];
      uint8_t map_[64];

    public:
      inline void reset() { memcpy(map_,empty_,64); }

      inline uint8_t get(size_t s) const
      {
        size_t pos      = s&0x1ff;
        size_t offset   = pos>>3;
        uint8_t bitpos  = pos&0x7;
        return ((map_[offset])>>bitpos)&1;
      }
      inline void set(size_t s)
      {
        size_t pos      = s&0x1ff;
        size_t offset   = pos>>3;
        uint8_t bitpos  = pos&0x7;
        map_[offset]   |= static_cast<uint8_t>(1<<bitpos);
      }
      inline void clear(size_t s)
      {
        size_t pos      = s&0x1ff;
        size_t offset   = pos>>3;
        uint8_t bitpos  = pos&0x7;
        map_[offset]   &= static_cast<uint8_t>(~(1<<bitpos));
      }
      inline size_t first_free() const
      {
        for( size_t i=0;i<64;++i )
        {
          uint8_t ff = first_free_[map_[i]];
          if( ff != 255 ) return ((i<<3)+ff);
        }
        return 512;
      }
      inline size_t find_n_free(size_t n) const
      {
        if(n>=512) return 512;
        if(n>8)
        {
          size_t blocks = ((n+7)>>3)<<3;
          size_t found = 0;
          size_t start = 65;
          for( size_t i=0;i<64;++i )
          {
            if(map_[i]==0)
            {
              if( start == 65 ) start = i;
              ++found;
              if( found == blocks ) return (start*8);
            }
            else
            {
              found = 0;
              start = 65;
            }
          }
        }
        else
        {
          for( size_t i=0;i<64;++i )
          {
            uint8_t ff = max_free_[map_[i]];
            if(ff>= n) return ((i<<3)+max_pos_[map_[i]]);
          }
        }
        return 512;
      }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_bitmap512_hh_included_ */
