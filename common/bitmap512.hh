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
#include "codesloop/common/bithacks.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class bitmap512
    {
    public:
      CSL_CLASS( csl::common::bitmap512 );
      typedef unsigned int pos_t;
      typedef uint32_t     item_t;

      inline bitmap512() : map_({0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0}) {}

      inline void reset() { memcpy(map_,empty_,64); }

      inline bool get(pos_t s) const
      {
        pos_t pos         = (s&0x1ff);
        uint8_t offset    = static_cast<uint8_t>(pos>>5);
        uint8_t bitpos    = static_cast<uint8_t>(pos&0x1f);
        return ((map_[offset])>>bitpos)&1;
      }

      inline void set(pos_t s)
      {
        pos_t pos         = (s&0x1ff);
        uint8_t offset    = static_cast<uint8_t>(pos>>5);
        uint8_t bitpos    = static_cast<uint8_t>(pos&0x1f);
        map_[offset]     |= static_cast<item_t>(1<<bitpos);
      }

      inline void clear(pos_t s)
      {
        pos_t pos         = (s&0x1ff);
        uint8_t offset    = static_cast<uint8_t>(pos>>5);
        uint8_t bitpos    = static_cast<uint8_t>(pos&0x1f);
        map_[offset]     &= static_cast<item_t>(~(1<<bitpos));
      }

      inline pos_t first_clear() const
      {
        for( pos_t i=0;i<16;++i )
        {
          uint32_t x = map_[i];
          if( x == 0xffffffff ) continue;
          uint32_t last_zero_bit = ((~x)&(x+1));
          return (i<<5)+bithacks::ntz(last_zero_bit);
        }
        return 512;
      }

      inline pos_t flip_first_clear()
      {
        for( pos_t i=0;i<16;++i )
        {
          uint32_t x = map_[i];
          if( x == 0xffffffff ) continue;
          uint32_t last_zero_bit = ((~x)&(x+1));
          map_[i] |= last_zero_bit;
          return (i<<5)+bithacks::ntz(last_zero_bit);
        }
        return 512;
      }

    private:
      static uint32_t empty_[16];
      item_t map_[16];
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_bitmap512_hh_included_ */
