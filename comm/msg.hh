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

#ifndef _csl_comm_msg_hh_included_
#define _csl_comm_msg_hh_included_
#include "codesloop/common/common.h"
#include "codesloop/common/dbc.hh"

namespace csl
{
  namespace comm
  {
    class msg
    {
    public:
      typedef struct sockaddr_in inaddr4_t;

      CSL_CLASS( csl::comm::msg );

      inline msg(inaddr4_t * addr, uint8_t * buf)
        : addr4_(addr), buffer_(buf), len_(0)
      {
        CSL_REQUIRE(addr != NULL);
        CSL_REQUIRE(buf != NULL);
      }

      inline void len(size_t l) { len_ = l; }
      inline msg & operator=(size_t l) { len_ = l; return *this; }

      inline size_t len() const { return len_; }
      inline const uint8_t * buffer() const { return buffer_; }
      inline const inaddr4_t & addr4() const { return *addr4_; }

      inline ~msg() {}

    private:
      inaddr4_t *  addr4_;
      uint8_t *    buffer_;
      size_t       len_;

      msg() = delete;
    };
  }
}

#endif /*_csl_comm_msg_hh_included_*/
