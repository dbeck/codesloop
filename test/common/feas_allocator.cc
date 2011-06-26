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

#ifdef CSL_DEBUG
#define CSL_DEBUG_ENABLE_INDENT
#define CSL_DEBUG_VERBOSE
#endif /* CSL_DEBUG */

#include "codesloop/common/common.h"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/stpodary.hh"
#include "codesloop/common/bitmap512.hh"
#include <vector>
#include <limits>

namespace test_allocator
{
  template<typename T>
  class dummy {
  public :
    //	typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

  public :
    //	convert an allocator<T> to allocator<U>
    template<typename U> struct rebind { typedef dummy<U> other; };

  public :
    inline explicit dummy() {}
    inline ~dummy() {}
    inline explicit dummy(dummy const&) {}
    template<typename U> inline explicit dummy(dummy<U> const&) {}

    //	address
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //	memory allocation
    inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
      return reinterpret_cast<pointer>(0);
    }
    inline void deallocate(pointer p, size_type) {}

    //	size
    inline size_type max_size() const {
      return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    //	construction/destruction
    inline void construct(pointer p, const T& t) { }
    inline void destroy(pointer p) { }

    inline bool operator==(dummy const&)   { return true; }
    inline bool operator!=(dummy const& a) { return !operator==(a); }
  };

  template<typename T, size_t SZ>
  class simple {
  public :
    //	typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

  public :
    //	convert an allocator<T> to allocator<U>
    template<typename U> struct rebind { typedef simple<U,SZ> other; };

  public :
    inline explicit simple() : len_(0) {}
    inline ~simple() {}
    inline explicit simple(simple const&) : len_(0) {}
    template<typename U> inline explicit simple(simple<U,SZ> const&) : len_(0) {}

    //	address
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //	memory allocation
    inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
      pointer ret = reinterpret_cast<pointer>(buf_+len_);
      len_+=cnt;
      return ret;
    }
    inline void deallocate(pointer p, size_type s) { len_ -= s; }

    //	size
    inline size_type max_size() const {
      return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    //	construction/destruction
    inline void construct(pointer p, const T& t) { new (p) T(t); }
    inline void destroy(pointer p) { p->~T(); }

    inline bool operator==(simple const&)   { return true; }
    inline bool operator!=(simple const& a) { return !operator==(a); }

  private:
    T      buf_[SZ];
    size_t len_;
  };

  void baseline()      { std::vector<int> v; }
  void baseline_16()   { std::vector<int> v; v.reserve(16);  }
  void baseline_256()  { std::vector<int> v; v.reserve(256); }

  void baseline_dummy()      { std::vector< int,dummy<int> > v; }
  void baseline_16_dummy()   { std::vector< int,dummy<int> > v; v.reserve(16); }
  void baseline_256_dummy()  { std::vector< int,dummy<int> > v; v.reserve(256); }

  void baseline_stp()      { csl::common::stpodary< int,1 >   v; }
  void baseline_16_stp()   { csl::common::stpodary< int,16 >  v; }
  void baseline_256_stp()  { csl::common::stpodary< int,256 > v; }

  void baseline_simple()
  {
    simple<int,1> a;
    std::vector< int,simple<int,1> > v(a);
  }

  void baseline_16_simple()
  {
    simple<int,16> a;
    std::vector< int,simple<int,16> > v(a);
    v.reserve(16);
  }

  void baseline_256_simple()
  {
    simple<int,256> a;
    std::vector< int,simple<int,256> > v(a);
    v.reserve(256);
  }

} // test_allocator

using namespace test_allocator;

int main()
{
  csl_common_print_results( "dummy                ", csl_common_test_timer_v0(baseline_dummy),"" );
  csl_common_print_results( "16_dummy             ", csl_common_test_timer_v0(baseline_16_dummy),"" );
  csl_common_print_results( "256_dummy            ", csl_common_test_timer_v0(baseline_256_dummy),"" );

  csl_common_print_results( "vector               ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "16_vector            ", csl_common_test_timer_v0(baseline_16),"" );
  csl_common_print_results( "256_vector           ", csl_common_test_timer_v0(baseline_256),"" );

  csl_common_print_results( "stpodary             ", csl_common_test_timer_v0(baseline_stp),"" );
  csl_common_print_results( "16_stpodary          ", csl_common_test_timer_v0(baseline_16_stp),"" );
  csl_common_print_results( "256_stpodary         ", csl_common_test_timer_v0(baseline_256_stp),"" );

  csl_common_print_results( "simple               ", csl_common_test_timer_v0(baseline_simple),"" );
  csl_common_print_results( "16_simple            ", csl_common_test_timer_v0(baseline_16_simple),"" );
  csl_common_print_results( "256_simple           ", csl_common_test_timer_v0(baseline_256_simple),"" );


  return 0;
}

// EOF
