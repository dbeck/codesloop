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

#ifndef _csl_common_pbuf_hh_included_
#define _csl_common_pbuf_hh_included_

#include "codesloop/common/pvlist.hh"
#include "codesloop/common/mpool.hh"
#include "codesloop/common/obj.hh"
#include "codesloop/common/serializable.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class pbuf : public obj, public serializable
    {
      public:
        enum { buf_size = 2048 };

        pbuf();
        ~pbuf() {}

        struct buf
        {
          unsigned char * data_;
          unsigned int    size_;

          unsigned int free_space()   { return buf_size - size_; }
          unsigned char * data_here() { return (size_ == buf_size ? 0 : (data_+size_)); }

          buf() : data_(0), size_(0) {}
        };

        bool append(const unsigned char * dta, uint64_t sz);

        bool append(const char * str)
        {
          uint64_t l=0;
          if( !str || (l=::strlen(str))== 0 ) return false;
          return append( reinterpret_cast<const unsigned char *>(str), (l+1) );
        }

        bool append(const wchar_t * str)
        {
          uint64_t l=0;
          if( !str || (l=::wcslen(str))== 0 ) return false;
          return append( reinterpret_cast<const unsigned char *>(str),(l+1)*sizeof(wchar_t));
        }

        pbuf & operator<<(const char * str) { append(str); return *this; }
        pbuf & operator<<(const wchar_t * str) { append(str); return *this; }

        template <typename T> bool t_copy_to(T & t, uint64_t max_size=0)
        {
          if( !max_size ) max_size = size();
          if( max_size )
          {
            unsigned char * bf = t.allocate(max_size);
            return copy_to(bf,max_size);
          }
          return false;
        }

        bool copy_to(unsigned char * ptr, uint64_t max_size=0) const;

        inline uint64_t size() const   { return size_; }

        inline uint64_t n_bufs() const { return bufpool_.n_items(); }

        typedef pvlist< 32,buf,delete_destructor<buf> > bufpool_t;
        typedef bufpool_t::iterator iterator;
        typedef bufpool_t::const_iterator const_iterator;

        inline iterator begin() { return bufpool_.begin(); }
        inline iterator end()   { return bufpool_.end();   }
        inline iterator last()  { return bufpool_.last();  }

        inline const_iterator begin() const { return bufpool_.begin(); }
        inline const_iterator end() const   { return bufpool_.end();   }
        inline const_iterator last() const  { return bufpool_.last();  }

        inline const_iterator const_begin() const { return bufpool_.const_begin(); }
        inline const_iterator const_end() const   { return bufpool_.const_end();   }
        inline const_iterator const_last() const  { return bufpool_.const_last();  }

        inline void free_all()
        {
          bufpool_.free_all();
          pool_.free_all();
          size_ = 0;
        }

        bool operator==(const pbuf & other) const;

        pbuf(const pbuf & other);
        pbuf & operator=(const pbuf & other);

        virtual void serialize(archiver & ar);
      private:
        buf * allocate(uint64_t size);

        /* variables */
        uint64_t        size_;
        unsigned char   preallocated_[buf_size];
        bufpool_t       bufpool_;
        mpool<>         pool_;
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_pbuf_hh_included_ */
