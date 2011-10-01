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

#ifndef _csl_comm_addr_hh_included_
#define _csl_comm_addr_hh_included_
#include "codesloop/common/common.h"

namespace csl
{
  namespace comm
  {
    class addr
    {
    public:
      inline addr()
        : addr_(reinterpret_cast<struct sockaddr *>(&addr_stor_)),
          len_(0)
      {
      }

      inline addr(const addr & other) { *this = other; }

      inline addr& operator=(const addr & other)
      {
        ::memcpy(&addr_stor_,&other.addr_stor_,sizeof(addr_stor_));
        addr_ = reinterpret_cast<struct sockaddr *>(&addr_stor_);
        len_  = other.len_;
        return *this;
      }

      inline ~addr() {}

      inline struct sockaddr * get()  { return addr_; }
      inline socklen_t & len()        { return len_; }

      inline const struct sockaddr * get() const { return addr_; }
      inline const socklen_t & len()       const { return len_; }

      inline addr & len(socklen_t l) { len_ = l; return *this; }

    private:
      struct sockaddr_storage  addr_stor_;
      struct sockaddr *        addr_;
      socklen_t                len_;
    };
  }
}

#endif /*_csl_comm_addr_hh_included_*/
