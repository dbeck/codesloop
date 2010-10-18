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

#ifndef _csl_common_pvlist_hh_included_
#define _csl_common_pvlist_hh_included_

#include <stdlib.h>
#include <stdio.h>
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    template <typename T>
    struct nop_destructor
    {
      inline void destruct(T ** p, size_t n) { }
      inline void destruct(T * p) { }
    };

    template <typename T>
    struct free_destructor
    {
      inline void destruct(T * p)
      {
        if( p ) ::free( p );
      }

      inline void destruct(T ** p, size_t n) 
      {
        for( size_t i=0;i<n;++i )
          if( p[i] ) 
            ::free( p[i] ); 
      }
    };

    template <typename T>
    struct delete_destructor
    {
      inline void destruct(T * p)
      {
        if( p ) delete p;
      }

      inline void destruct(T ** p, size_t n) 
      {
        for( size_t i=0;i<n;++i )
          if( p[i] ) 
            delete p[i]; 
      }
    };

    template <
      size_t   I=64,
      typename T=void,
      typename D=nop_destructor<T> >
    class pvlist
    {
    public:
      enum { block_size = I };
      typedef T item_type_t;

      inline size_t bs() { return block_size; }

      struct iterator;
      friend struct iterator;

    private:
      struct item
      {
        T *       ptrs_[block_size];
        size_t    used_;
        item *    next_;

        inline item( ) : used_(0), next_(0) { }
      };

      item   head_;
      item * tail_;
      size_t n_items_;

      inline item * ensure_item()
      {
        if( tail_->used_ == block_size )
        {
          item * old_tail = tail_;
          tail_           = new item();
          old_tail->next_ = tail_;
        }
        return tail_;
      }

    public:
      class iterator
      {
      private:
        item * i_;
        size_t pos_;

        iterator() {}
      public:
        ~iterator() {}

        iterator(item * i, size_t pos) : i_(i), pos_(pos) {}
        iterator(const iterator & other) : i_(other.i_), pos_(other.pos_) {}
        iterator & operator=(const iterator & other)
        {
          i_   = other.i_;
          pos_ = other.pos_;
          return *this;
        }

        iterator(pvlist & ls) : i_(&(ls.head_)), pos_(0)
        {
          {
            if(!ls.n_items_) i_ = 0;
          }
        }

        bool operator==(const iterator & other)
        {
          return ((i_ == other.i_ && pos_ == other.pos_) ? true : false );
        }

        bool operator!=(const iterator & other)
        {
          return (!(operator==(other)));
        }

        void operator++()
        {
          if( i_ == 0 )
          {
            i_   = 0;
            pos_ = 0;
          }
          else if( pos_ == (i_->used_-1) )
          {
            i_   = i_->next_;
            pos_ = 0;
          }
          else
          {
            ++pos_;
          }
        }

        void zero() { i_ = 0; pos_ = 0; }

        item_type_t * operator*()
        {
          if( i_ ) { return i_->ptrs_[pos_]; }
          else     { return 0;               }
        }

        void free()
        {
          D d;
          if( i_ && i_->ptrs_[pos_] )
          {
            d.destruct( i_->ptrs_[pos_] );
            i_->ptrs_[pos_] = 0;
          }
        }
      };

      class const_iterator
      {
        private:
          const item * i_;
          size_t pos_;

          const_iterator() {}
        public:
          ~const_iterator() {}

          const_iterator(const item * i, size_t pos) : i_(i), pos_(pos) {}
          const_iterator(const const_iterator & other) : i_(other.i_), pos_(other.pos_) {}
          const_iterator & operator=(const const_iterator & other)
          {
            i_   = other.i_;
            pos_ = other.pos_;
            return *this;
          }

          const_iterator(const pvlist & ls) : i_(&(ls.head_)), pos_(0)
          {
            if(!ls.n_items_) i_ = 0;
          }

          bool operator==(const const_iterator & other) const
          {
            return ((i_ == other.i_ && pos_ == other.pos_) ? true : false );
          }

          bool operator!=(const const_iterator & other) const
          {
            return (!(operator==(other)));
          }

          void operator++()
          {
            if( i_ == 0 )
            {
              i_   = 0;
              pos_ = 0;
            }
            else if( pos_ == (i_->used_-1) )
            {
              i_   = i_->next_;
              pos_ = 0;
            }
            else
            {
              ++pos_;
            }
          }

          void zero() { i_ = 0; pos_ = 0; }
          const item_type_t * operator*() const
          {
            if( i_ ) { return i_->ptrs_[pos_]; }
            else     { return 0;               }
          }
      };

      iterator begin()
      {
        iterator ret(*this);
        return ret;
      }

      iterator end()
      {
        iterator ret(*this);
        ret.zero();
        return ret;
      }

      iterator last()
      {
        if( !n_items_ || !tail_ || !(tail_->used_) )
        {
          return end();
        }
        else
        {
          iterator ret(tail_,tail_->used_-1);
          return ret;
        }
      }

      const_iterator const_begin() const
      {
        const_iterator ret(*this);
        return ret;
      }

      const_iterator begin() const { return const_begin(); }
      const_iterator const_end() const
      {
        const_iterator ret(*this);
        ret.zero();
        return ret;
      }

      const_iterator end() const { return const_end(); }
      const_iterator const_last() const
      {
        if( !n_items_ || !tail_ || !(tail_->used_) )
        {
          return end();
        }
        else
        {
          const_iterator ret(tail_,tail_->used_-1);
          return ret;
        }
      }

      const_iterator last() const { return const_last(); }

    public:
      inline pvlist() : tail_(&head_), n_items_(0) { }
      inline ~pvlist()
      {
        free_all();
      }

      inline size_t n_items() const { return n_items_; }
      inline size_t size() const { return n_items_; }
      inline void free_all()
      {
        D d;
        d.destruct( head_.ptrs_, head_.used_ );
        item * i = head_.next_;
        while( i )
        {
          item * o = i;
          i = i->next_;
          d.destruct( o->ptrs_, o->used_ );
          delete o;
        }
        head_.used_ = 0;
        head_.next_ = 0;
        tail_ = &head_;
        n_items_ = 0;
      }

      inline bool find(T * p)
      {
        item * i = &head_;

        while( i )
        {
          for( size_t ii=0;ii<(i->used_); ++ii )
          {
            if( i->ptrs_[ii] == p )
            {
              return true;
            }
          }
          i = i->next_;
        }
        return false;
      }

      inline bool free(T * p)
      {
        bool freed=false;
        D d;
        item * i = &head_;

        while( i )
        {
          for( size_t ii=0;ii<(i->used_); ++ii )
          {
            if( i->ptrs_[ii] == p )
            {
              if( !freed )
              {
                d.destruct( i->ptrs_[ii] );
                freed = true;
              }
              i->ptrs_[ii] = 0;
            }
          }
          i = i->next_;
        }
        return freed;
      }

      inline bool free_one(T * p)
      {
        D d;
        item * i = &head_;

        while( i )
        {
          for( size_t ii=0;ii<(i->used_); ++ii )
          {
            if( i->ptrs_[ii] == p )
            {
              d.destruct( i->ptrs_[ii] );
              i->ptrs_[ii] = 0;
              return true;
            }
          }
          i = i->next_;
        }
        return false;
      }

      inline T * push_back(T * p)
      {
        item * i = ensure_item();
        i->ptrs_[i->used_] = p;
        ++(i->used_);
        ++n_items_;
        return p;
      }

      inline T * get_at(size_t which) const
      {
        if( which >= n_items_ ) return 0;

        size_t pn = which/block_size;

        /* first page go fast */
        if( !pn  )
          return (which < head_.used_ ? head_.ptrs_[which] : 0);

        /* iterate through pages */
        const item * pt = &head_;
        while( pt->next_ && pn > 0 )
        {
          --pn;
          pt = pt->next_;
        }

        /* not enough pages */
        if( pn )
          return 0;

        size_t rm = which%block_size;

        return (rm < pt->used_ ? pt->ptrs_[rm] : 0);
      }

      inline bool set_at(size_t which, T * ptr)
      {
        if( which >= n_items_ ) return false;

        size_t pn = which/block_size;

        /* first page go fast */
        if( !pn  ) 
        {
          if( which >= head_.used_ ) return false;
          head_.ptrs_[which] = ptr;
          return true;
        }

        /* iterate through pages */
        item * pt = &head_;
        while( pt->next_ && pn > 0 )
        {
          --pn;
          pt = pt->next_;
        }

        /* not enough pages */
        if( pn )
          return false;

        size_t rm = which%block_size;

        if( rm < pt->used_ )
        {
          pt->ptrs_[rm] = ptr;
          return true;
        }
        else
        {
          return false;
        }
      }

      inline void debug()
      {
        printf("== pvlist::debug ==\n");
        item * pt = &head_;
        while( pt )
        {
          printf(
            "  item:\n"
            "  -- ptrs : %p\n"
            "  -- used : %ld\n"
            "  -- next : %p\n"
            "  ---- : ",
                 pt->ptrs_,
                 static_cast<unsigned long>(pt->used_),
                 pt->next_ );
          for( size_t i=0;i<pt->used_;++i ) printf("%p ",pt->ptrs_[i]);
          printf("\n");
          pt = pt->next_;
        }
      }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_pvlist_hh_included_ */
