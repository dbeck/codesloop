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

#ifndef _csl_common_stpodary_impl_hh_included_
#define _csl_common_stpodary_impl_hh_included_
#ifdef __cplusplus

#include <new>

namespace csl
{
  namespace common
  {
    template <typename T,size_t SZ>
    T * stpodary<T,SZ>::allocate(size_t sz)
    {
      T * ret = data_;

      if( !sz )
      {
        reset();
      }
      else if( sz <= size_ )
      {
        size_ = sz;
      }
      else if( sz <= SZ && is_static() )
      {
        size_ = sz;
      }
      else
      {
        T * tmp = new (std::nothrow) T[sz]; 

        if( !tmp )
        {
          ret = 0;
        }
        else
        {
          if( size_ > 0 )
          {
            ::memcpy( tmp, data_, size_ );
            if( is_dynamic() ) delete [] data_;
          }

          data_ = tmp;
          size_ = sz;
          ret = data_;
        }
      }
      return ( ret );
    }

    template <typename T,size_t SZ>
    void stpodary<T,SZ>::reset()
    {
      if( is_dynamic() )
      {
        delete [] data_;
        data_ = preallocated_;
      }
      size_ = 0;
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ>::~stpodary()
    {
      reset();
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ>::stpodary()
        : data_(preallocated_), size_(0)
    {
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ>::stpodary(const T & c)
        : data_(preallocated_), size_(1)
    {
      preallocated_[0] = c;
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ>::stpodary(const stpodary & other)
        : data_(preallocated_), size_(0)
    {
      *this = other;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::set(const T * dta, size_t sz)
    {
      /* if no data on the other side we are done */
      if( !sz )  { reset(); return true; }

      /* if sz is not zero than dta must not be null */
      if( !dta ) { return false; }

      if( allocate(sz) )
      {
        /* copy in the data */
        ::memcpy( data_, dta, item_size_ * sz );
        return true;
      }
      else
      {
        return false;
      }
    }

    template <typename T, size_t SZ>
    T * stpodary<T,SZ>::allocate_nocopy(size_t sz)
    {
      if( !sz ) { reset(); return data_; }

      if( sz <= size_ )
      {
        /* the requested data is smaller than the allocated */
        size_ = sz;
        return data_;
      }
      else if( sz <= SZ )
      {
        /* data fits into preallocated size */
        if( size_ > 0 && is_dynamic() ) delete [] data_;

        data_ = preallocated_;
        size_ = sz;
        return data_;
      }
      else
      {
        /* cannot use the preallocated space */
        T * tmp = new (std::nothrow) T[sz];
        if( !tmp ) return 0;

        /* already have data ? */
        if( size_ > 0 && is_dynamic() ) delete [] data_; 

        data_ = tmp;
        size_ = sz;
        return data_;
      }
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ> &
    stpodary<T,SZ>::operator=(const stpodary & other)
    {
      /* return immediately if they are the same */
      if( &other == this || other.data_ == data_ )
      {
        return *this;
      }

      /* quick return if empty */
      if( other.is_empty() ) { reset(); return *this; }

      T * tmp = allocate_nocopy( other.size_ );

      if( tmp )
      {
        ::memcpy( tmp, other.data_, other.size_*item_size_ );
      }
      return *this;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::operator==(const stpodary & other) const
    {
      if( other.size_ != size_ ) return false;
      if( size_ == 0 )           return true;
      if( data_ == 0 )           return false;
      if( other.data_ == 0 )     return false;
      return (::memcmp( other.data_, data_, size_*item_size_ ) == 0);
    }

    template <typename T, size_t SZ>
    T & stpodary<T,SZ>::operator[](size_t at)
    {
       return data_[at];
    }

    template <typename T, size_t SZ>
    const T & stpodary<T,SZ>::operator[](size_t at) const
    {
      return data_[at];
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::stpodary<T,SZ>::get(T * dta) const
    {
      if( !dta || !size_ || !data_ ) { return false; }
      ::memcpy( dta,data_,size_*item_size_ );
      return true;
    }

    template <typename T, size_t SZ>
    void stpodary<T,SZ>::append(const T & c)
    {
      set_at(size_,c);
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::append(const T * dta, size_t sz)
    {
      /* if no data on the other side we are done */
      if( !sz )  { return true; }

      /* if sz is not zero than dta must not be null */
      if( !dta ) { return false; }

      if( allocate(size_+sz) )
      {
        /* copy in the data */
        ::memcpy( data_+size_-sz, dta, sz*item_size_ );
        return true;
      }
      else
      {
        return false;
      }
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::append(const stpodary & other)
    {
      return append( other.data(), other.size() );
    }

    template <typename T, size_t SZ>
    void stpodary<T,SZ>::set_at(size_t pos,const T & c)
    {
      T * t = data_;
      if( pos >= size_ ) t = allocate( pos+1 );
      t[pos] = c;
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_stpodary_impl_hh_included_ */
