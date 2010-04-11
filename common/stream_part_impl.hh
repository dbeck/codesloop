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

#if 0
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
//#define DEBUG_VERBOSE
#endif /* DEBUG */
#endif /* 0 */

#include "codesloop/common/logger.hh"
#include "codesloop/common/str.hh"

namespace csl
{
  namespace common
  {
    void stream_flags_to_str(uint32_t f, str & s)
    {
      s += L"[";        
      if( f&(1ULL<<0) )  s+= L" timed_out_";
      if( f&(1ULL<<1) )  s+= L" closed_";
      if( f&(1ULL<<2) )  s+= L" stopped_";
      if( f&(1ULL<<3) )  s+= L" permission_error_";
      if( f&(1ULL<<4) )  s+= L" not_authenticated_";
      if( f&(1ULL<<5) )  s+= L" not_authorized_";
      if( f&(1ULL<<6) )  s+= L" security_error_";
      if( f&(1ULL<<7) )  s+= L" parameter_error_";
      if( f&(1ULL<<8) )  s+= L" data_error_";
      if( f&(1ULL<<9) )  s+= L" crc_error_";
      if( f&(1ULL<<10) ) s+= L" serialization_error_";
      if( f&(1ULL<<11) ) s+= L" deserialization_error_";
      if( f&(1ULL<<12) ) s+= L" try_again_";
      if( f&(1ULL<<13) ) s+= L" out_of_memory_";
      if( f&(1ULL<<14) ) s+= L" buffer_full_";
      if( f&(1ULL<<15) ) s+= L" too_fast_";
      if( f&(1ULL<<16) ) s+= L" too_slow_";
      if( f&(1ULL<<17) ) s+= L" too_large_";
      if( f&(1ULL<<18) ) s+= L" too_small_";
      if( f&(1ULL<<19) ) s+= L" read_only_";
      if( f&(1ULL<<20) ) s+= L" write_only_";
      if( f&(1ULL<<21) ) s+= L" read_error_";
      if( f&(1ULL<<22) ) s+= L" write_error_";
      if( f&(1ULL<<23) ) s+= L" out_of_range_";
      if( f&(1ULL<<24) ) s+= L" database_error_";
      if( f&(1ULL<<25) ) s+= L" network_error_";
      if( f&(1ULL<<26) ) s+= L" application_error_";
      if( f&(1ULL<<27) ) s+= L" os_error_";
      if( f&(1ULL<<28) ) s+= L" peer_exception_";
      s += L" ]";
    }

    template<typename T>
    stream_part<T>::stream_part()
        : data_(0), bytes_(0), flags_(ok_) { }

    template<typename T>
    void stream_part<T>::reset()
    {
      ENTER_FUNCTION();
      data_       = 0;
      bytes_      = 0;
      flags_      = ok_;
      LEAVE_FUNCTION();
    }

    template<typename T>
    T * stream_part<T>::data() const
    {
      ENTER_FUNCTION();
      RETURN_FUNCTION( data_ );
    }

    template<typename T>
    void stream_part<T>::data(T * d)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"data(%p)",d );
      data_ = d;
      LEAVE_FUNCTION();
    }

    template<typename T>
    size_t stream_part<T>::bytes() const
    {
      ENTER_FUNCTION();
      RETURN_FUNCTION( bytes_ );
    }

    template<typename T>
    void stream_part<T>::bytes(size_t b)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF(L"bytes(%lld)",static_cast<uint64_t>(b) );
      bytes_ = b;
      LEAVE_FUNCTION();
    }
    
    template<typename T>
    uint32_t stream_part<T>::flags() const
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str;
      stream_flags_to_str(flags_,flags_str);
      CSL_DEBUGF(L"flags() => %x:%ls",flags_,flags_str.c_str() );
#endif /*DEBUG*/
      RETURN_FUNCTION(flags_);
    }
    
    template<typename T>
    bool stream_part<T>::has_flags(uint32_t fl) const
    {
      ENTER_FUNCTION();
      bool ret = ((flags_ & fl) == fl);
#ifdef DEBUG
      str flags_str, fl_str;
      stream_flags_to_str(flags_,flags_str);
      stream_flags_to_str(fl,fl_str);
      CSL_DEBUGF(L"has_flags(%x:%ls) [flags:%x:%ls] => %ls",
        fl,fl_str,
        flags_,flags_str.c_str(),
        (ret?L"TRUE":L"FALSE") );
#endif /*DEBUG*/
      RETURN_FUNCTION(ret);
    }
    
    template<typename T>
    void stream_part<T>::set_flags(uint32_t fl)
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str, fl_str;
      stream_flags_to_str(flags_,flags_str);
      stream_flags_to_str(fl,fl_str);
      CSL_DEBUGF(L"set_flags(%x:%ls) [old_flags:%x:%ls]",
        fl,fl_str,
        flags_,flags_str.c_str());
#endif /*DEBUG*/
      flags_ = fl;
      LEAVE_FUNCTION();
    }
    
    template<typename T>
    void stream_part<T>::clear_flags(uint32_t fl)
    {
      ENTER_FUNCTION();
      uint32_t new_flags = (flags_ & (~fl));
#ifdef DEBUG
      str flags_str, fl_str, new_flags_str;
      stream_flags_to_str(flags_,flags_str);
      stream_flags_to_str(fl,fl_str);
      stream_flags_to_str(new_flags,new_flags_str);
      CSL_DEBUGF(L"clear_flags(%x:%ls) [old_flags:%x:%ls] [new_flags:%x:%ls]",
        fl,fl_str,
        flags_,flags_str.c_str(),
        new_flags,new_flags_str.c_str() );
#endif /*DEBUG*/
      flags_ = new_flags;
      LEAVE_FUNCTION();
    }
    
    template<typename T>
    void stream_part<T>::add_flags(uint32_t fl)
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str, fl_str;
      stream_flags_to_str(flags_,flags_str);
      stream_flags_to_str(fl,fl_str);
      CSL_DEBUGF(L"set_flags(%x:%ls) [old_flags:%x:%ls]",
        fl,fl_str,
        flags_,flags_str.c_str());
#endif /*DEBUG*/
      flags_ |= fl;
      LEAVE_FUNCTION();
    }
    
  } /* end of ns: csl::common */
} /* end of ns: csl */

/* EOF */
