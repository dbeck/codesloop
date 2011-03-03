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

#ifndef _csl_nthread_mutex_hh_included_
#define _csl_nthread_mutex_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"

#ifdef __cplusplus
#include <memory>

namespace csl
{
  namespace nthread
  {
    class mutex
    {
    public:
      mutex();
      ~mutex();
      bool lock(unsigned long timeout_ms=0);
      bool try_lock();
      bool unlock();
      bool recursive_unlock();
      bool is_locked();
      unsigned int waiting_count();
      unsigned int locked_count();

    private:
      struct impl;
      std::auto_ptr<impl> impl_;

      // no-copy
      mutex(const mutex & other);
      mutex & operator=(const mutex & other);

      CSL_OBJ(csl::nthread, mutex);
      USE_EXC();
    };

    template <typename T=mutex>
    class scoped_mutex_template
    {
    public:
      scoped_mutex_template(T & mtx) : mtx_(&mtx)
      {
        mtx_->lock();
      }

      ~scoped_mutex_template()
      {
        mtx_->unlock();
      }

    private:
      T * mtx_;

      // no default construction and copying
      scoped_mutex_template() : mtx_(0) {}
      scoped_mutex_template(const scoped_mutex_template &) : mtx_(0) {}
      scoped_mutex_template & operator=(const scoped_mutex_template &) { return *this; }

      CSL_OBJ(csl::nthread, scoped_mutex_template);
    };

    typedef scoped_mutex_template<mutex> scoped_mutex;

    class ondemand_mutex
    {
    public:
      ondemand_mutex() : mtx_(0) {}
      ~ondemand_mutex() { if( mtx_ ) { delete mtx_; }; mtx_ = 0; }

      bool lock(unsigned long timeout_ms=0) { init(); return mtx_->lock(timeout_ms); }
      bool try_lock() { init(); return mtx_->try_lock(); }
      bool unlock() { init(); return mtx_->unlock(); }
      bool recursive_unlock() { init(); return mtx_->recursive_unlock(); }
      bool is_locked() { init(); return mtx_->is_locked(); }
      unsigned int waiting_count() { init(); return mtx_->waiting_count(); }
      unsigned int locked_count() { init(); return mtx_->locked_count(); }

    private:
      void init() { if( !mtx_ ) mtx_ = new mutex(); }

      mutex * mtx_;

      // no copying
      ondemand_mutex(const ondemand_mutex &) : mtx_(0) {}
      ondemand_mutex & operator=(const ondemand_mutex &) { return *this; }

      CSL_OBJ(csl::nthread, scoped_mutex_template);
    };

    typedef scoped_mutex_template<ondemand_mutex> scoped_ondemand_mutex;
  }
}

#endif /* __cplusplus */
#endif /* _csl_nthread_mutex_hh_included_ */

/* EOF */
