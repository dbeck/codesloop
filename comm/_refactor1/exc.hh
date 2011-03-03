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

#ifndef _csl_comm_exc_hh_included_
#define _csl_comm_exc_hh_included_

#include "codesloop/common/str.hh"
#include "codesloop/common/cexc.hh"
#ifdef __cplusplus

namespace csl
{
  namespace comm
  {
    class exc : public csl::common::cexc
    {
      public:
        enum {
          rs_unknown,
          rs_socket_failed,
          rs_bind_failed,
          rs_listen_failed,
          rs_accept_failed,
          rs_connect_failed,
          rs_pubkey_empty,
          rs_privkey_empty,
          rs_xdr_error,
          rs_send_failed,
          rs_recv_failed,
          rs_timeout,
          rs_select_failed,
          rs_thread_start,
          rs_wsa_startup,
          rs_getsockname_failed,
          rs_invalid_packet_type,
          rs_common_error,
          rs_sec_error,
          rs_crypt_pkt_error,
          rs_too_big,
          rs_pkt_error,
          rs_null_param,
          rs_not_inited,
          rs_init_failed,
          rs_need_login,
          rs_need_pass,
          rs_sesskey_empty,
          rs_salt_size,
          rs_not_implemented,
          rs_setsockopt,
          rs_assert,
          rs_unknown_op,
          rs_internal_state,
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
#endif /* _csl_comm_exc_hh_included_ */
