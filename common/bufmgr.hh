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

#ifndef _csl_common_bufmgr_hh_included_
#define _csl_common_bufmgr_hh_included_
#include "codesloop/common/common.h"
#include "codesloop/common/dbc.hh"
#include "codesloop/common/bitmap512.hh"

namespace csl
{
  namespace common
  {
    class bufmgr
    {
    public:
      CSL_CLASS( csl::common::bufmgr );
      CSL_DECLARE_EXCEPTION( out_of_memory );
      static const uint32_t buf_size_ = 4096;

      class item
      {
      public:
        CSL_CLASS( csl::common::bufmgr::item );
        friend class csl::common::bufmgr;

        inline item(item && x) : id_(x.id_), data_(x.data_), used_(x.used_), mgr_(x.mgr_)
        {
          x.id_   = 512;
          x.data_ = 0;
          x.used_ = 0;
          x.mgr_  = 0;
        }

        inline item & operator=(item && x)
        {
          CSL_REQUIRE( this != &x );
          if (this != &x)
          {
            if(mgr_) mgr_->free(*this);

            id_     = x.id_;
            data_   = x.data_;
            used_   = x.used_;
            mgr_    = x.mgr_;

            x.id_   = 512;
            x.data_ = 0;
            x.used_ = 0;
            x.mgr_  = 0;
          }
          return *this;
        }

        inline ~item() { if(mgr_) mgr_->free(*this); }
        inline item() : id_(512), data_(0), used_(0), mgr_(0)  {}

        inline uint8_t*       operator->()       { return data_; }
        inline const uint8_t* operator->() const { return data_; }

        inline void           used(uint32_t s)   { used_ = s;    }
        inline uint32_t       used() const       { return used_; }

        inline uint32_t       max_size() const   { return bufmgr::buf_size_; }

        inline item clone() const
        {
          item ret;
          if( mgr_ )
          {
            ret = mgr_->alloc();
            if(used_)
            {
              ret.used_ = used_;
              memcpy(ret.data_,data_,used_);
            }
          }
          return ret;
        }

      private:
        bitmap512::pos_t  id_;
        uint8_t *         data_;
        uint32_t          used_;
        bufmgr *          mgr_;
      };

      inline item alloc()
      {
        bitmap512::pos_t id=map_.flip_first_clear();

        if( id == 512 ) { CSL_THROW( out_of_memory ); }

        item i;

        i.id_   = id;
        i.data_ = pool_+(id*buf_size_);
        i.used_ = 0;
        i.mgr_  = this;

        return i;
      }

      inline void free(item & i)
      {
        CSL_REQUIRE( i.id_ < 512 );
        CSL_REQUIRE( i.mgr_ != 0 );
        CSL_REQUIRE( i.data_ != 0 );
        CSL_REQUIRE( i.used_ <= buf_size_ );

        map_.clear(i.id_);

        i.id_   = 512;
        i.data_ = 0;
        i.used_ = 0;
        i.mgr_  = 0;
      }

      inline bufmgr() {}
      inline ~bufmgr() {}

    private:
      uint8_t      pool_[512*buf_size_];
      bitmap512    map_;

      bufmgr(const bufmgr &) = delete;
      bufmgr & operator=(const bufmgr &) = delete;
    };
  }
}

#endif /*_csl_common_bufmgr_hh_included_*/
