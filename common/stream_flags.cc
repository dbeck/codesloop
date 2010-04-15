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

#include "stream_flags.hh"

namespace csl
{
  namespace common
  {
    void stream_flags::to_str(str & s)
    {
      stream_flags::to_str(flags_,s);
    }

    void stream_flags::to_str(uint32_t f, str & s)
    {
      s += L"[";
      if( f == 0 ) s+= L" ok_ ";
      else
      {
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
      }
      s += L" ]";
    }
  
    uint32_t stream_flags::flags() const
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str;
      to_str(flags_,flags_str);
      CSL_DEBUGF(L"flags() => %x:%ls",flags_,flags_str.c_str() );
#endif /*DEBUG*/
      RETURN_FUNCTION(flags_);
    }
    
    bool stream_flags::has_flags(uint32_t fl) const
    {
      ENTER_FUNCTION();
      bool ret = ((flags_ & fl) == fl);
#ifdef DEBUG
      str flags_str, fl_str;
      to_str(flags_,flags_str);
      to_str(fl,fl_str);
      CSL_DEBUGF(L"has_flags(%x:%ls) [flags:%x:%ls] => %ls",
        fl,fl_str,
        flags_,flags_str.c_str(),
        (ret?L"TRUE":L"FALSE") );
#endif /*DEBUG*/
      RETURN_FUNCTION(ret);
    }
    
    void stream_flags::set_flags(uint32_t fl)
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str, fl_str;
      to_str(flags_,flags_str);
      to_str(fl,fl_str);
      CSL_DEBUGF(L"set_flags(%x:%ls) [old_flags:%x:%ls]",
        fl,fl_str,
        flags_,flags_str.c_str());
#endif /*DEBUG*/
      flags_ = fl;
      LEAVE_FUNCTION();
    }
    
    void stream_flags::clear_flags(uint32_t fl)
    {
      ENTER_FUNCTION();
      uint32_t new_flags = (flags_ & (~fl));
#ifdef DEBUG
      str flags_str, fl_str, new_flags_str;
      to_str(flags_,flags_str);
      to_str(fl,fl_str);
      to_str(new_flags,new_flags_str);
      CSL_DEBUGF(L"clear_flags(%x:%ls) [old_flags:%x:%ls] [new_flags:%x:%ls]",
        fl,fl_str,
        flags_,flags_str.c_str(),
        new_flags,new_flags_str.c_str() );
#endif /*DEBUG*/
      flags_ = new_flags;
      LEAVE_FUNCTION();
    }
    
    void stream_flags::add_flags(uint32_t fl)
    {
      ENTER_FUNCTION();
#ifdef DEBUG
      str flags_str, fl_str;
      to_str(flags_,flags_str);
      to_str(fl,fl_str);
      CSL_DEBUGF(L"set_flags(%x:%ls) [old_flags:%x:%ls]",
        fl,fl_str,
        flags_,flags_str.c_str());
#endif /*DEBUG*/
      flags_ |= fl;
      LEAVE_FUNCTION();
    }
    
    bool stream_flags::operator==(uint32_t other)
    {
      ENTER_FUNCTION();
      bool ret = (flags_ == other);
#ifdef DEBUG
      str flags_str, fl_str;
      to_str(flags_,flags_str);
      to_str(other,other_str);
      CSL_DEBUGF(L"operator==(%x:%ls) [flags:%x:%ls] => %ls",
        other,other_str,
        flags_,flags_str.c_str(),
        (ret?L"TRUE":L"FALSE"));
#endif /*DEBUG*/      
      RETURN_FUNCTION(ret);
    }
    
    bool stream_flags::operator==(const stream_flags & other)
    {
      ENTER_FUNCTION();
      bool ret = (flags_ == other.flags_);
#ifdef DEBUG
      str flags_str, fl_str;
      to_str(flags_,flags_str);
      to_str(other.flags_,other_str);
      CSL_DEBUGF(L"operator==(%x:%ls) [flags:%x:%ls] => %ls",
        other.flags_,other_str,
        flags_,flags_str.c_str(),
        (ret?L"TRUE":L"FALSE"));
#endif /*DEBUG*/      
      RETURN_FUNCTION(ret);
    }
    
    bool stream_flags::operator!=(uint32_t other)
    {
      ENTER_FUNCTION();
      bool ret = (operator==(other) == false);
      RETURN_FUNCTION(ret);
    }
    
    bool stream_flags::operator!=(const stream_flags & other)
    {
      ENTER_FUNCTION();
      bool ret = (operator==(other) == false);
      RETURN_FUNCTION(ret);
    }

    stream_flags & stream_flags::operator<<(uint32_t other)
    {
      ENTER_FUNCTION();
      add_flags(other);
      RETURN_FUNCTION(*this);
    }
    
    stream_flags & stream_flags::operator<<(const stream_flags & other)
    {
      ENTER_FUNCTION();
      add_flags(other.flags_);
      RETURN_FUNCTION(*this);
    }
    
    stream_flags & stream_flags::operator=(uint32_t other)
    {
      ENTER_FUNCTION();
      set_flags(other);
      RETURN_FUNCTION(*this);
    }

    stream_flags & stream_flags::operator=(const stream_flags & other)
    {
      ENTER_FUNCTION();
      set_flags(other.flags_);
      RETURN_FUNCTION(*this);
    }

  } /* end of ns:csl:common */
} /* end of ns:csl */

// EOF

