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

  class bitmap512
  {
  private:
    static uint8_t empty_[64];
    static uint8_t first_free_[256];
    uint8_t map_[64];

  public:
    inline void reset() { memcpy(map_,empty_,64); }

    inline uint8_t get(size_t s) const
    {
      size_t pos      = s&0x1ff;
      size_t offset   = pos>>3;
      uint8_t bitpos  = pos&0x7;
      return ((map_[offset])>>bitpos)&1;
    }
    inline void set(size_t s)
    {
      size_t pos      = s&0x1ff;
      size_t offset   = pos>>3;
      uint8_t bitpos  = pos&0x7;
      map_[offset]   |= static_cast<uint8_t>(1<<bitpos);
    }
    inline void clear(size_t s)
    {
      size_t pos      = s&0x1ff;
      size_t offset   = pos>>3;
      uint8_t bitpos  = pos&0x7;
      map_[offset]   &= static_cast<uint8_t>(~(1<<bitpos));
    }
    inline size_t first_free() const
    {
      for( size_t i=0;i<64;++i )
      {
        uint8_t ff = first_free_[map_[i]];
        if( ff != 255 ) return ((i<<3)+ff);
      }
      return 512;
    }

  };

  uint8_t bitmap512::empty_[64] =
  {
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
  };

  uint8_t bitmap512::first_free_[256] =
  {
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,6,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,7,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,6,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,5,
    0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,4, 0,1,0,2,0,1,0,3, 0,1,0,2,0,1,0,255
  };

  void b512()       { bitmap512 b; }
  void b512_reset() { bitmap512 b; b.reset(); }
  void b512_pos()   { bitmap512 b; b.reset(); b.set(13); b.get(13); }
  void b512_ff()    { bitmap512 b; b.reset(); b.set(0);  b.first_free(); }

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
  bitmap512 b;
  b.reset();
  for( size_t i=0;i<400;++i )
  {
    for( size_t j=0;j<512;++j )
    {
      if(b.get(j)!=0)
      {
        printf("A%ld@%ld ",j,i);
      }
    }
    b.set(i);
    if( b.get(i)!=1 )
    {
      printf("B%ld ",i);
    }
    for( size_t j=0;j<512;++j )
    {
      if(j!=i && b.get(j)!=0)
      {
        printf("C%ld@%ld(b[j]=%d) ",j,i,b.get(j));
      }
    }
    b.clear(i);
    for( size_t j=0;j<512;++j )
    {
      if(b.get(j)!=0)
      {
        printf("D%ld@%ld ",j,i);
      }
    }
    for( size_t j=0;j<512;++j ) { b.set(j); }
    b.clear(i);
    if( b.first_free()!=i )
    {
      printf("E%ld ",i);
    }
    b.reset();
  }

  csl_common_print_results( "b512                 ", csl_common_test_timer_v0(b512),"" );
  csl_common_print_results( "b512_reset           ", csl_common_test_timer_v0(b512_reset),"" );
  csl_common_print_results( "b512_pos             ", csl_common_test_timer_v0(b512_pos),"" );
  csl_common_print_results( "b512_ff              ", csl_common_test_timer_v0(b512_ff),"" );

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
