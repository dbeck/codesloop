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

#ifndef _csl_common_mpool_hh_included_
#define _csl_common_mpool_hh_included_

#include "codesloop/common/pvlist.hh"
#include "codesloop/common/common.h"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <typename P=pvlist< 64,void,free_destructor<void> > >
    class mpool
    {
    private:
      P v_;

      mpool(const mpool & other) {}
      mpool & operator=(const mpool & other) { return *this; }

    public:
      inline mpool() {}
      inline void * allocate(uint64_t len) // TODO : change to size_t
      {
        if( !len ) { return 0; }
        void * ret = ::malloc( static_cast<size_t>(len) );
        v_.push_back(ret);
        return ret;
      }

      inline void * get_at(uint64_t which) const
      {
        return v_.get_at(which);
      }

      inline char * strdup(const char * str)
      {
        if( !str ) return 0;
        uint64_t len = ::strlen(str);
        char * ret = 0;
        ret = reinterpret_cast<char *>(allocate(len+1));
        if( len ) ::memcpy( ret, str, static_cast<size_t>(len) );
        ret[len] = 0;
        return ret;
      }

      inline wchar_t * wcsdup(const wchar_t * str)
      {
        if( !str ) return 0;
        uint64_t len = ::wcslen(str);
        wchar_t * ret = 0;
        ret = reinterpret_cast<wchar_t *>(allocate( (len+1)*sizeof(wchar_t) ));
        if( len )
        {
          wcsncpy(ret, str, static_cast<size_t>(len));
          ret[len] = L'\0';
        }
        return ret;
      }

      inline void * memdup(const void * ptr, uint64_t sz)
      {
        if( !ptr || !sz ) return 0;
        void * ret = 0;
        ret = allocate(sz);
        ::memcpy( ret, ptr, static_cast<size_t>(sz) );
        return ret;
      }

      inline void debug()
      {
        v_.debug();
      }

      inline void free_all()
      {
        v_.free_all();
      }

      inline bool free(void * p)
      {
        return v_.free_one(p);
      }

      inline bool is_from(void * p)
      {
        return v_.find(p);
      }

      inline bool find(void * p)
      {
        return v_.find(p);
      }
    };

    typedef mpool<> default_mpool_t;
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_mpool_hh_included_ */

