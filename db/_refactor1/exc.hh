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

#ifndef _csl_slt3_exc_hh_included_
#define _csl_slt3_exc_hh_included_

#include "codesloop/common/cexc.hh"
#include "codesloop/common/str.hh"
#ifdef __cplusplus

namespace csl
{
  namespace db
  {
    class exc : public csl::common::cexc
    {
      public:
        enum {
          rs_unknown,   
          rs_notopened,
          rs_nullparam,
          rs_nulltran,
          rs_nullconn,
          rs_nulldb,
          rs_nullstmnt,
          rs_transaction_already_started,
          rs_internal,
          rs_permission,
          rs_abort,
          rs_busy,
          rs_locked,
          rs_nomem,
          rs_readonly,
          rs_ioerr,
          rs_corrupt,
          rs_notfound,
          rs_cantopen,
          rs_full,
          rs_protocol,
          rs_empty,
          rs_schema,
          rs_toobig,
          rs_constraint,
          rs_mismatch,
          rs_misuse,
          rs_auth,
          rs_format,
          rs_range,
          rs_notadb,
          rs_cannot_reg,
          rs_not_implemented,
          rs_mysql_init,
          rs_mysql_real_connect,
          rs_mysql_outofmem,
          rs_empty_query,
          rs_cannot_prepare,
          rs_invalid_param,
          rs_mysql_stmt_bind,
          rs_mysql_stmt_execute,
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
#endif /* _csl_slt3_exc_hh_included_ */
