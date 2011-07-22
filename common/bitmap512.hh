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

      inline void set_n(pos_t s, pos_t n)
      {
         // XXX TODO
      }

      inline void clear(pos_t s)
      {
        pos_t pos         = (s&0x1ff);
        uint8_t offset    = static_cast<uint8_t>(pos>>5);
        uint8_t bitpos    = static_cast<uint8_t>(pos&0x1f);
        map_[offset]     &= static_cast<item_t>(~(1<<bitpos));
      }

      inline void clear_n(pos_t s, pos_t n)
      {
         // XXX TODO
      }

      inline pos_t first_clear() const
      {
        for( pos_t i=0;i<16;++i )
        {
          uint8_t ff0 = first_free_[(map_[i])&0xff];
          if( ff0 != 255 ) return ((i<<5)+ff0);

          uint8_t ff1 = first_free_[((map_[i])>>8)&0xff];
          if( ff1 != 255 ) return ((i<<5)+8+ff1);

          uint8_t ff2 = first_free_[((map_[i])>>16)&0xff];
          if( ff2 != 255 ) return ((i<<5)+16+ff2);

          uint8_t ff3 = first_free_[((map_[i])>>24)&0xff];
          if( ff3 != 255 ) return ((i<<5)+24+ff3);
        }
        return 512;
      }

      inline pos_t flip_first_clear()
      {
        for( pos_t i=0;i<16;++i )
        {
          uint8_t ff0 = first_free_[(map_[i])&0xff];
          if( ff0 != 255 )
          {
            pos_t ret = ((i<<5)+ff0);
            set(ret);
            return ret;
          }

          uint8_t ff1 = first_free_[((map_[i])>>8)&0xff];
          if( ff1 != 255 )
          {
            pos_t ret = ((i<<5)+8+ff1);
            set(ret);
            return ret;
          }

          uint8_t ff2 = first_free_[((map_[i])>>16)&0xff];
          if( ff2 != 255 )
          {
            pos_t ret = ((i<<5)+16+ff2);
            set(ret);
            return ret;
          }

          uint8_t ff3 = first_free_[((map_[i])>>24)&0xff];
          if( ff3 != 255 )
          {
            pos_t ret = ((i<<5)+24+ff3);
            set(ret);
            return ret;
          }
        }
        return 512;
      }

      inline pos_t find_n_clear(pos_t n) const
      {
        if(n>=512) return 512;
        if(n>8)
        {
          pos_t blocks = ((n+31)>>5)<<5;
          pos_t found = 0;
          pos_t start = 128;
          for( pos_t i=0;i<16;++i )
          {
            if(map_[i]==0)
            {
              if( start == 128 ) start = i;
              ++found;
              if( found == blocks ) return (start*32);
            }
            else
            {
              found = 0;
              start = 128;
            }
          }
        }
        else
        {
          for( pos_t i=0;i<64;++i )
          {
            uint8_t ff = max_free_[map_[i]];
            if(ff>= n) return ((i<<3)+max_pos_[map_[i]]);
          }
        }
        /*
        if(n>8)
        {
          pos_t blocks = ((n+7)>>3)<<3;
          pos_t found = 0;
          pos_t start = 65;
          for( pos_t i=0;i<64;++i )
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
          for( pos_t i=0;i<64;++i )
          {
            uint8_t ff = max_free_[map_[i]];
            if(ff>= n) return ((i<<3)+max_pos_[map_[i]]);
          }
        }
        */
        return 512;
      }

    private:
      static uint8_t empty_[64];
      static uint8_t first_free_[256];
      static uint8_t max_free_[256];
      static uint8_t max_pos_[256];
      static uint8_t max_posB_[256];
      item_t map_[16];
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_bitmap512_hh_included_ */
