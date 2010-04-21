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

#ifndef _csl_common_stream_flags_hh_included_
#define _csl_common_stream_flags_hh_included_

#include "codesloop/common/exc.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/logger.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/obj.hh"

#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class stream_flags
    {
      public:
        // error flags
        static const uint32_t ok_                     = 0UL;
        static const uint32_t timed_out_              = (1UL<<0);
        static const uint32_t closed_                 = (1UL<<1);
        static const uint32_t stopped_                = (1UL<<2);
        static const uint32_t permission_error_       = (1UL<<3);
        static const uint32_t not_authenticated_      = (1UL<<4);
        static const uint32_t not_authorized_         = (1UL<<5);
        static const uint32_t security_error_         = (1UL<<6);
        static const uint32_t parameter_error_        = (1UL<<7);
        static const uint32_t data_error_             = (1UL<<8);
        static const uint32_t crc_error_              = (1UL<<9);
        static const uint32_t serialization_error_    = (1UL<<10);
        static const uint32_t deserialization_error_  = (1UL<<11);
        static const uint32_t try_again_              = (1UL<<12);
        static const uint32_t out_of_memory_          = (1UL<<13);
        static const uint32_t buffer_full_            = (1UL<<14);
        static const uint32_t too_fast_               = (1UL<<15);
        static const uint32_t too_slow_               = (1UL<<16);
        static const uint32_t too_large_              = (1UL<<17);
        static const uint32_t too_small_              = (1UL<<18);
        static const uint32_t read_only_              = (1UL<<19);
        static const uint32_t write_only_             = (1UL<<20);
        static const uint32_t read_error_             = (1UL<<21);
        static const uint32_t write_error_            = (1UL<<22);
        static const uint32_t out_of_range_           = (1UL<<23);
        static const uint32_t database_error_         = (1UL<<24);
        static const uint32_t network_error_          = (1UL<<25);
        static const uint32_t application_error_      = (1UL<<26);
        static const uint32_t os_error_               = (1UL<<27);
        static const uint32_t peer_exception_         = (1UL<<28);
        static const uint32_t empty_buffer_           = (1UL<<29);
                
        stream_flags() : flags_(ok_) {}
        
        uint32_t flags() const;
        bool has_flags(uint32_t flags) const;
        
        void set_flags(uint32_t f);
        void clear_flags(uint32_t f);
        void add_flags(uint32_t f);
        
        bool operator==(uint32_t other) const;
        bool operator==(const stream_flags & other) const;
        bool operator!=(uint32_t other) const;
        bool operator!=(const stream_flags & other) const;
        bool operator&(uint32_t other) const;
        bool operator&(const stream_flags & other) const;

        stream_flags & operator<<(uint32_t other);
        stream_flags & operator<<(const stream_flags & other);
        stream_flags & operator=(uint32_t other);
        stream_flags & operator=(const stream_flags & other);
        
        static void to_str(uint32_t f, str & s);
        void to_str(str & s);
        
      private:
        uint32_t flags_;
        CSL_OBJ(csl::common, stream_flags);
    };
  } /* end of ns:csl:common */
} /* end of ns:csl */

#endif /* __cplusplus */
#endif /* _csl_common_stream_flags_hh_included_ */

