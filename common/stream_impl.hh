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

#ifndef _csl_common_stream_impl_hh_included_
#define _csl_common_stream_impl_hh_included_
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
  
#ifdef DEBUG
#define DEBUG_FLAGS_AND_TARGETOP(fun) \
            str flags_str___; flags_.to_str(flags_str___); \
            CSL_DEBUGF(fun L"() => stream flags:[%x:%ls]", \
                      flags_.flags(),flags_str___.c_str() ); \
#else
#define DEBUG_FLAGS_AND_TARGETOP(fun)
#endif

    template <typename T, typename Target, typename Buffer>
    const stream_flags &
    stream<T,Target,Buffer>::start()
    {
      ENTER_FUNCTION();
      if( !target_ ) RETURN_FUNCTION(flags_);
      flags_ << target_->start(*this);
      DEBUG_FLAGS_AND_TARGETOP(L"start");
      RETURN_FUNCTION(flags_);
    }
    
    template <typename T, typename Target, typename Buffer>
    const stream_flags &
    stream<T,Target,Buffer>::end()
    {
      ENTER_FUNCTION();
      if( !target_ ) RETURN_FUNCTION(flags_);
      flags_ << target_->end(*this);
      DEBUG_FLAGS_AND_TARGETOP(L"end");
      RETURN_FUNCTION(flags_);
    }
    
    template <typename T, typename Target, typename Buffer>
    const stream_flags &
    stream<T,Target,Buffer>::flush()
    {
      ENTER_FUNCTION();
      if( !target_ ) RETURN_FUNCTION(flags_);
      flags_ << target_->flush(*this);
      DEBUG_FLAGS_AND_TARGETOP(L"flush");
      RETURN_FUNCTION(flags_);
    }
                
    template <typename T, typename Target, typename Buffer>
    stream_flags &
    stream<T,Target,Buffer>::flags()
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str; flags_.to_str(flags_str);
      CSL_DEBUGF(L"flags() => flags:[%x:%ls]",
        flags_.flags(),flags_str.c_str() );
#endif /*DEBUG*/
      RETURN_FUNCTION(flags_);
    }

#undef DEBUG_FLAGS_AND_TARGETOP
#ifdef DEBUG
#define DEBUG_FLAGS_AND_BUFFEROP(fun,param) \
            str flags_str___; flags_.to_str(flags_str___); \
            CSL_DEBUGF(fun L"(%lld,sp) => sp:[data:%p,bytes:%d] stream flags:[%x:%ls]", \
                      static_cast<uint64_t>(param), \
                      sp.data(), sp.bytes(), \
                      flags_.flags(),flags_str___.c_str() ); \
#else
#define DEBUG_FLAGS_AND_BUFFEROP(fun,param)
#endif

    template <typename T, typename Target, typename Buffer>
    typename stream<T,Target,Buffer>::part_t &
    stream<T,Target,Buffer>::reserve( size_t sz,
                                      stream<T,Target,Buffer>::part_t & sp )
    {
      ENTER_FUNCTION();
      part_t & ret(buffer_->reserve(sz,sp));
      flags_ << sp.flags();
      sp.flags() << flags_;
      DEBUG_FLAGS_AND_BUFFEROP(L"reserve",sz);
      RETURN_FUNCTION(ret);
    }
    
    template <typename T, typename Target, typename Buffer>
    typename stream<T,Target,Buffer>::part_t &
    stream<T,Target,Buffer>::confirm( size_t n_succeed,
                                      stream<T,Target,Buffer>::part_t & sp )
    {
      ENTER_FUNCTION();
      part_t & ret(buffer_->confirm(n_succeeded,sp));
      flags_ << sp.flags();
      if(target_)
      {
        flags_ << target_->data(*this,n_succeed);
      }
      sp.flags() << flags_;
      DEBUG_FLAGS_AND_BUFFEROP(L"confirm",n_succeed);
      RETURN_FUNCTION(ret);
    }
    
    template <typename T, typename Target, typename Buffer>
    typename stream<T,Target,Buffer>::part_t &
    stream<T,Target,Buffer>::get( size_t sz,
                                  stream<T,Target,Buffer>::part_t & sp )
    {
      ENTER_FUNCTION();
      part_t & ret(buffer_->get(sz,sp));
      flags_ << sp.flags();
      sp.flags() << flags_;
      DEBUG_FLAGS_AND_BUFFEROP(L"get",n_succeed);
      RETURN_FUNCTION(ret);
    }

#undef DEBUG_FLAGS_AND_BUFFEROP

    template <typename T, typename Target, typename Buffer>
    size_t
    stream<T,Target,Buffer>::confirmed_items()
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"confirmed_items() => %lld",static_cast<uint64_t>(n_confirmed_));
      RETURN_FUNCTION(n_confirmed_);
    }
    
    template <typename T, typename Target, typename Buffer>
    size_t
    stream<T,Target,Buffer>::has_items()
    {
      ENTER_FUNCTION();
      size_t ret = buffer_->has_items();
      CSL_DEBUGF(L"has_items() => %lld",static_cast<uint64_t>(ret));
      RETURN_FUNCTION(ret);
    }

    template <typename T, typename Target, typename Buffer>
    stream<T,Target,Buffer>::stream(Target & t, Buffer & b) : target_(&t),
                                                              buffer_(&b),
                                                              n_confirmed_(0),
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"stream::stream(target:%p,buffer:%p)",&t,&b);
      LEAVE_FUNCTION();
    }
    
    template <typename T, typename Target, typename Buffer>
    stream<T,Target,Buffer>::stream(Buffer & b) : target_(0),
                                                  buffer_(&b),
                                                  n_confirmed_(0)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"stream::stream(buffer:%p)",&b);
      LEAVE_FUNCTION();
    }
    
    template <typename T, typename Target, typename Buffer>
    void stream<T,Target,Buffer>::set_target(Target & t)
    {
      ENTER_FUNCTION();
      target_ = &t;
      CSL_DEBUGF(L"set_target(target:%p)",&t);
      LEAVE_FUNCTION();
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_stream_impl_hh_included_ */

