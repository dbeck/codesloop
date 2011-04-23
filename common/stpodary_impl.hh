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

#ifndef _csl_common_stpodary_impl_hh_included_
#define _csl_common_stpodary_impl_hh_included_
#ifdef __cplusplus

#include <new>

namespace csl
{
  namespace common
  {

#ifdef CSL_DEBUG
    template <typename T,size_t SZ>
    bool stpodary<T,SZ>::csl_invariant() const
    {
      if( size_ > SZ && data_ == preallocated_ ) return false;
      if( data_ == NULL ) return false;
      return true;
    }
#endif //CSL_DEBUG

    template <typename T,size_t SZ>
    T * stpodary<T,SZ>::allocate(size_t sz)
    {
      T * ret = data_;

      if( !sz )
      {
        reset();
        ret = data_;
      }
      //
      // the second part will will work becuse if it is (or was) dynamic
      // then the allocated size is > SZ if static then SZ elements
      // are akready allocated dynamically
      //
      else if( sz <= size_ || sz <= SZ )
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
            ::memcpy(tmp,data_,(size_*item_size_));
            if( is_dynamic() ) delete [] data_;
          }

          data_ = tmp;
          size_ = sz;
          ret = data_;
          CSL_ENSURE( is_dynamic() == true );
          CSL_ENSURE( is_static() == false );
        }
      }
      CSL_CHECK_INVARIANT();
      return ( ret );
    }

    template <typename T, size_t SZ>
    T * stpodary<T,SZ>::allocate_nocopy(size_t sz)
    {
      T * ret = data_;
      
      if( !sz )
      {
        reset();
        ret = data_;
      }
      else if( sz <= size_ )
      {
        // the requested data is smaller than the allocated
        size_ = sz;
        ret = data_;
      }
      else if( sz <= SZ )
      {
        // data fits into preallocated size
        if( size_ > 0 && is_dynamic() ) delete [] data_;

        ret = data_ = preallocated_;
        size_ = sz;
      }
      else
      {
        // cannot use the preallocated space
        T * tmp = new (std::nothrow) T[sz];
        if( !tmp )
        {
          ret = 0;
        }
        else
        {
          // already have data ?
          if( size_ > 0 && is_dynamic() ) delete [] data_; 

          ret = data_ = tmp;
          size_ = sz;
          CSL_ENSURE( is_dynamic() == true );
          CSL_ENSURE( is_static() == false );
        }
      }

      CSL_CHECK_INVARIANT();
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
      CSL_ENSURE( is_dynamic() == false );
      CSL_ENSURE( is_static() == true );
      CSL_ENSURE( is_empty() == true );
      CSL_CHECK_INVARIANT();
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
      CSL_ENSURE( is_static() == true );
      CSL_ENSURE( is_dynamic() == false );
      CSL_ENSURE( is_empty() == true );
      CSL_ENSURE( has_data() == false );
      CSL_CHECK_INVARIANT();
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ>::stpodary(const T & c)
        : data_(preallocated_), size_(1)
    {
      preallocated_[0] = c;
      CSL_ENSURE( is_static() == true );
      CSL_ENSURE( is_dynamic() == false );
      CSL_ENSURE( is_empty() == false );
      CSL_ENSURE( has_data() == true );
      CSL_CHECK_INVARIANT();      
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ>::stpodary(const stpodary & other)
        : data_(preallocated_), size_(0)
    {
      *this = other;
      CSL_ENSURE( is_static() == other.is_static() );
      CSL_ENSURE( is_dynamic() == other.is_dynamic() );
      CSL_ENSURE( is_empty() == other.is_empty() );
      CSL_ENSURE( has_data() == other.has_data() );
      CSL_ENSURE( size() == other.size() );
      CSL_CHECK_INVARIANT();
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::set(const T * dta, size_t sz)
    {
      // if no data on the other side we are done
      if( !sz )  { reset(); return true; }

      CSL_REQUIRE( dta != NULL );

      // if sz is not zero than dta must not be null
      if( !dta ) { return false; }

      bool ret = (allocate(sz) != NULL);
      
      if( ret )
      {
        // copy in the data
        ::memcpy(data_,dta,(item_size_*sz));
      }
      
      CSL_CHECK_INVARIANT();
      return ret;
    }

    template <typename T, size_t SZ>
    stpodary<T,SZ> &
    stpodary<T,SZ>::operator=(const stpodary & other)
    {
      CSL_REQUIRE( &other != this && other.data_ != data_ );
    
      // return immediately if they are the same
      if( &other == this || other.data_ == data_ )
      {
        return *this;
      }

      // quick return if empty
      if( other.is_empty() ) { reset(); return *this; }

      T * tmp = allocate_nocopy( other.size_ );

      if( tmp )
      {
        ::memcpy(tmp,other.data_,(other.size_*item_size_));
      }
      else
      {
        CSL_THROW( out_of_memory );
      }

      CSL_CHECK_INVARIANT();
      return *this;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::operator==(const stpodary & other) const
    {
      CSL_REQUIRE( other.data_ != NULL );
      CSL_REQUIRE( data_ != NULL );

      if( &other == this )       return true;
      if( other.size_ != size_ ) return false;
      if( other.data_ == data_ ) return true;
      if( size_ == 0 )           return true;
      if( data_ == 0 )           return false;
      if( other.data_ == 0 )     return false;

      return (::memcmp( other.data_, data_, size_*item_size_ ) == 0);
    }

    template <typename T, size_t SZ>
    T & stpodary<T,SZ>::operator[](size_t at)
    {
      CSL_REQUIRE( at < size_ );
      return data_[at];
    }

    template <typename T, size_t SZ>
    const T & stpodary<T,SZ>::operator[](size_t at) const
    {
      CSL_REQUIRE( at < size_ );
      return data_[at];
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::stpodary<T,SZ>::get(T * dta) const
    {
      CSL_REQUIRE( dta != NULL );

      if( !dta || !size_ || !data_ ) { return false; }

      ::memcpy(dta,data_,(size_*item_size_));
      
      CSL_CHECK_INVARIANT();
      return true;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::append(const T & c)
    {
      bool ret = set_at(size_,c);

      CSL_ENSURE( is_empty() == false );
      CSL_ENSURE( has_data() == true );
      CSL_CHECK_INVARIANT();
      return ret;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::append(const T * dta, size_t sz)
    {
      // if no data on the other side we are done
      if( !sz )  { return true; }
      
      CSL_REQUIRE( dta != NULL );

      // if sz is not zero than dta must not be null
      if( !dta ) { return false; }
      
      bool ret = (allocate(size_+sz) != NULL);

      if( ret )
      {
        // copy in the data
        ::memcpy(data_+size_-sz,dta,(sz*item_size_));
      }
      
      CSL_CHECK_INVARIANT();
      return ret;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::append(const stpodary & other)
    {
      bool ret = append( other.data(), other.size() );
      CSL_CHECK_INVARIANT();
      return ret;
    }

    template <typename T, size_t SZ>
    bool stpodary<T,SZ>::set_at(size_t pos, const T & c)
    {
      T * t = data_;
      if( pos >= size_ ) t = allocate( pos+1 );
      if( !t ) return false;
      t[pos] = c;
      CSL_ENSURE( is_empty() == false );
      CSL_ENSURE( has_data() == true );
      CSL_CHECK_INVARIANT();
      return true;
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_stpodary_impl_hh_included_ */
