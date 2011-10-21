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

#ifndef _csl_common_kspin_hh_included_
#define _csl_common_kspin_hh_included_
#include "codesloop/common/excbase.hh"
#include <atomic>

namespace csl
{
  namespace common
  {
    class kspin
    {
    public:
      CSL_CLASS( csl::common::kspin );

      static const uint32_t init_ = 0x1;

      inline kspin(uint32_t init) : val_(init), lck_(ATOMIC_FLAG_INIT) { }
      inline kspin() : val_(init_), lck_(ATOMIC_FLAG_INIT) { }

      // wait if: val_==0
      // effect:  val_+=1
      inline uint32_t gtinc()
      {
        bool stop = false;
        uint32_t ret = 0;
        do
        {
          internal_lock();
          if( val_ )
          {
            // overflow
            if( val_ > 0xfffffff0 ) val_ = 0;
            stop = true;
            ret = (++val_);
          }
          internal_unlock();
        }
        while(stop == false);
        return ret;
      }

      // wait if:  val_==0
      // error if: val_!=id
      // effect:   val_=0
      inline bool lock(uint32_t id)
      {
        bool ret = true;
        bool stop = false;
        do
        {
          internal_lock();
          if( val_ == id )  { val_ = 0; stop = true; }
          else if( val_ )   { ret = false; }
          internal_unlock();
        }
        while(ret == true && stop == false);
        return ret;
      }

      // no wait for val_
      // assume the one who unlocks, did have the lock before
      // effect: val_=id
      inline void unlock(uint32_t id)
      {
        internal_lock();
        val_ = id;
        internal_unlock();
      }

      inline uint32_t load()
      {
        internal_lock();
        uint32_t ret = val_;
        internal_unlock();
        return ret;
      }

    private:
      uint32_t           val_;
      std::atomic_flag   lck_;

      inline void internal_lock()   { while(lck_.test_and_set(std::memory_order_acquire)); }
      inline void internal_unlock() { lck_.clear(std::memory_order_release); }
    };

    class kspin_lock
    {
    public:
      CSL_CLASS( csl::common::kspin_lock );

      kspin_lock(kspin & ks, uint32_t lockid) : lock_(&ks), id_(lockid) {}

      inline bool lock()          { return lock_->lock(id_); }
      inline void unlock()        { lock_->unlock(id_); }
      inline uint32_t id() const  { return id_; }

    private:
      kspin_lock() = delete;

      kspin *   lock_;
      uint32_t  id_;
    };

    class scoped_kspin_lock
    {
    public:
      CSL_CLASS( csl::common::scoped_kspin_lock );
      CSL_DECLARE_EXCEPTION( key_changed );

      scoped_kspin_lock(kspin_lock & ksl) : lock_(&ksl)
      {
        if( lock_->lock() == false ) { CSL_THROW( key_changed ); }
      }

      ~scoped_kspin_lock()
      {
        lock_->unlock();
      }

    private:
      scoped_kspin_lock() = delete;
      scoped_kspin_lock(const scoped_kspin_lock &) = delete;
      scoped_kspin_lock & operator=(const scoped_kspin_lock &) = delete;

      kspin_lock * lock_;
    };
  }
}

#endif /* _csl_common_kspin_hh_included_ */

