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

#ifndef _csl_common_lgr_msg_hh_included_
#define _csl_common_lgr_msg_hh_included_
#include "codesloop/common/lgr_loc.hh"
#include "codesloop/common/allocator.hh"
#include "codesloop/common/stpodary.hh"
#include <vector>

namespace csl
{
  namespace common
  {
    namespace lgr // logger helpers
    {
      class msg
      {
      public:
        CSL_CLASS( csl::common::lgr::msg );

        struct part
        {
          enum {
            tag_,       wtag_,
            name_,      wname_,
            string_,    wstring_,
            int32_,     uint32_,
            int64_,     uint64_,
            date_,      datetime_,      datetimems_,
            idate_,     idatetime_,     idatetimems_,
            double_
          };

          char type_;
          union {
            uint32_t str_ref_;
            int32_t  int32_data_;
            uint32_t uint32_data_;
            int64_t  int64_data_;
            uint64_t uint64_data_;
            double   double_data_;
          };
        };

        enum { preallocated_parts_=16,
               preallocated_buffer_=256 };

        typedef csl::common::allocator< part,
                                        preallocated_parts_,
                                        csl::common::impl::simpstack >  allocator_t;
        typedef std::vector< part, allocator_t >                        partvec_t;
        typedef csl::common::stpodary<char, preallocated_buffer_>       buffer_t;
        typedef csl::common::stpodary<wchar_t, preallocated_buffer_>    wbuffer_t;

        msg(lgr::loc & l) : loc_(&l) {}

        template <typename T> msg & operator<<(const T &) { return *this; }
      private:
        lgr::loc *      loc_;
        partvec_t       parts_;
        buffer_t        buffer_;
        wbuffer_t       wbuffer_;
      };
    }
  }
}

#endif //_csl_common_lgr_msg_hh_included_
