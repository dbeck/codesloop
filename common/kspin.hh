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

      inline kspin(uint32_t init) : spin_(init) { }
      inline kspin() : spin_(init_) { }

      // TODO : tryset(x)
      // if(spin_ != 0) { spin_ = x; return true; }
      // else           { return false; }

      // TODO : set(x)
      // while(spin_ == 0) { }
      // spin_ = x;

      // TODO : tryinc()
      // if(spin_ != 0) { ++spin_; return spin_; }
      // else           { return 0; }

      // TODO : inc()
      // while(spin_ == 0) {}
      // ++spin_; return spin_;

      // TODO : gtinc()
      // if(spin_ != 0) { ++spin_; return spin_; }
      // else           { return 0; }

      // TODO : trygtinc()
      // while(spin_ == 0) {}
      // ++spin_; return spin_;

      // TODO : tryunlock(x)
      // if(spin_ == 0) { spin_ = x; return true; }
      // else           { return false; }

      // old => new
      // locks the kspin to new id
      inline void xlock(uint32_t old_id, uint32_t new_id)
      {
        uint32_t old_=old_id;
        while( !spin_.compare_exchange_strong(old_,new_id) )
        {
          old_=old_id;
        }
      }

      // id => 0 except id mismatch
      inline bool lock(uint32_t id)
      {
        uint32_t id_=id;
        uint32_t busy=0;
        while( !spin_.compare_exchange_strong(id_,busy) )
        {
          if( id_!=id ) return false;
          id_=id;
        }
        return true;
      }

      // 0 => id
      inline void unlock(uint32_t id) { spin_.store(id); }

      inline uint32_t load() const { return spin_.load(); }

    private:
      std::atomic_uint_fast32_t spin_;
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

