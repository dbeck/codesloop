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

#ifndef _csl_sec_crypt_buf_hh_included_
#define _csl_sec_crypt_buf_hh_included_

#include "codesloop/common/common.h"
#ifdef __cplusplus
#include <memory>

#ifndef CSL_SEC_CRYPT_BUF_HEAD_LEN
#define CSL_SEC_CRYPT_BUF_HEAD_LEN 8
#endif /* CSL_SEC_CRYPT_BUF_HEAD_LEN */

#ifndef CSL_SEC_CRYPT_BUF_MAC_LEN
#define CSL_SEC_CRYPT_BUF_MAC_LEN 40
#endif /* CSL_SEC_CRYPT_BUF_HEAD_LEN */

namespace csl
{
  namespace sec
  {
    class crypt_buf
    {
    public:
      uint64_t get_header_len();
      uint64_t get_mac_len();

      bool init_crypt( unsigned char * buf,
                       const char * key,
                       bool encrypt,
                       const unsigned char * rndata=0 );

      bool init_crypt( unsigned char * buf,
                       const unsigned char * key,
                       uint64_t keylen,
                       bool encrypt,
                       const unsigned char * rndata=0);

      bool add_data(unsigned char * buf, uint64_t len, bool encrypt);

      bool finalize(unsigned char * outbuff);

      crypt_buf();
      ~crypt_buf();

      struct impl;
    private:
      /* private data */
      std::auto_ptr<impl> impl_;
      /* copying not allowed */
      crypt_buf(const crypt_buf & other);
      crypt_buf & operator=(const crypt_buf & other);
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_sec_crypt_buf_hh_included_ */
