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

#ifndef _csl_common_exc_hh_included_
#define _csl_common_exc_hh_included_

#include "codesloop/common/cexc.hh"
#include "codesloop/common/serializable.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/logger.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class exc : public csl::common::cexc
    {
      public:
        enum {
          rs_unknown,
          rs_invalid_param,
          rs_cannot_append,
          rs_cannot_get,
          rs_xdr_eof,
          rs_xdr_invalid,
          rs_empty,
          rs_conv_error,
          rs_invalid_state,
          rs_lookup_error,
          rs_out_of_memory,
          rs_assert,
          rs_stream_push,
          rs_stream_pop,
        };

        static const wchar_t * reason_string(int rc);

        exc(const wchar_t * component)
          : csl::common::cexc(component) {}

        exc(int reason, const wchar_t * component)
          : csl::common::cexc(reason,component) {}

        exc(int reason, const wchar_t * component, const wchar_t * txt)
          : csl::common::cexc(reason,component,txt) {}

        exc(int reason, const wchar_t * component, const wchar_t * txt, const wchar_t * file, unsigned int line)
          : csl::common::cexc(reason,component,txt,file,line) {}
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_exc_hh_included_ */
