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

#ifndef _csl_common_allocator_hh_included_
#define _csl_common_allocator_hh_included_
#include "codesloop/common/dbc.hh"
#include <limits>
#include <type_traits>

namespace csl
{
  namespace common
  {
    namespace impl
    {
      template <bool skip> struct destroy_op {
        template <typename T> inline void operator()(T* p) { p->~T(); }
      };

      template <> struct destroy_op<true> {
        template <typename T> inline void operator()(T* p) {}
      };

      template <bool skip> struct construct_op {
        template <typename T> inline void operator()(T * p, const T & t) { new (p) T(t); }
      };

      template <> struct construct_op<true> {
        template <typename T> inline void operator()(T * p, const T & t) {}
      };

      template <typename T, size_t SZ>
      struct stdc
      {
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef std::size_t size_type;

        inline pointer allocate(size_type cnt, const void * x = 0)
        {
          allocated_+=cnt;
          if( (allocated_) > SZ )
          {
            return static_cast<pointer>(malloc(cnt*sizeof(value_type)));
          }
          else
          {
            if( !bufp_ ) bufp_ = reinterpret_cast<pointer>(malloc(SZ*sizeof(value_type)));
            return bufp_+(allocated_-cnt);
          }
        }
        inline void deallocate(pointer p, size_type s)
        {
          if( p > bufp_+SZ || p < bufp_ ) free(p);
        }
        inline size_type allocated() { return allocated_; }
        inline stdc() : allocated_(0), bufp_(0) {}
        ~stdc() { if(bufp_) free(bufp_); }

        size_type     allocated_;
        pointer       bufp_;

        // TODO XXX : copy op and constructor
      };

      template <typename T, size_t SZ>
      struct simpstack
      {
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef std::size_t size_type;

        inline pointer allocate(size_type cnt, const void * x = 0)
        {
          allocated_+=cnt;
          if( (allocated_) > SZ )
          {
            return static_cast<pointer>(malloc(cnt*sizeof(value_type)));
          }
          return bufp_+(allocated_-cnt);
        }
        inline void deallocate(pointer p, size_type s)
        {
          if( p > bufp_+SZ || p < bufp_ ) free(p);
        }
        inline size_type allocated() { return allocated_; }
        inline simpstack() : allocated_(0), bufp_(reinterpret_cast<pointer>(buf_)) {}
        unsigned char buf_[SZ*sizeof(T)];
        size_type     allocated_;
        pointer       bufp_;

        // TODO XXX : copy op and constructor
      };
    }

    template<
      typename T,
      size_t SZ=256,
      template <class T2,size_t SZ2> class Impl=impl::stdc
      >
    class allocator {
    public :
      //  typedefs
      typedef T value_type;
      typedef value_type* pointer;
      typedef const value_type* const_pointer;
      typedef value_type& reference;
      typedef const value_type& const_reference;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;

      //  convert an allocator<T> to allocator<U>
      template<typename U> struct rebind { typedef allocator<U,SZ,Impl> other; };

      inline explicit allocator() {}
      inline ~allocator() {}

      inline pointer address(reference r) { return &r; }
      inline const_pointer address(const_reference r) { return &r; }

      inline size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
      }

      inline void construct(pointer p, const value_type& t) {
        impl::construct_op<std::is_pod<value_type>::value> do_it;
        do_it(p,t);
      }

      inline void destroy(pointer p) {
        impl::destroy_op<std::is_pod<value_type>::value> do_it;
        do_it(p);
      }

      inline pointer allocate(size_type cnt, const void * x = 0)
      {
        return impl_.allocate(cnt,x);
      }
      inline void deallocate(pointer p, size_type s)
      {
        impl_.deallocate(p,s);
      }

      inline explicit allocator(allocator const&)
      {
        // TODO XXX
      }

      template<typename U>
      inline explicit allocator(allocator<U,SZ,Impl> const&) { }

      inline bool operator==(allocator const&)   { return true; }
      inline bool operator!=(allocator const& a) { return !operator==(a); }

    private:
      Impl<T,SZ> impl_;
    };
  }
}

#endif /* _csl_common_allocator_hh_included_ */
