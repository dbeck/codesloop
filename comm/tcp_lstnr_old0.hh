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

#ifndef _csl_comm_tcp_lstnr_old0_hh_included_
#define _csl_comm_tcp_lstnr_old0_hh_included_

#include "codesloop/comm/sai.hh"
#include "codesloop/comm/connid.hh"
#include "codesloop/comm/handler_old0.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#include "codesloop/nthread/pevent.hh"
#ifdef __cplusplus
#include <memory>

namespace csl
{
  using nthread::pevent;

  namespace comm
  {
    namespace tcp
    {
      class lstnr_old0
      {
        public:
          lstnr_old0();
          virtual ~lstnr_old0();

          /* address, to be setup during initialization */
          const SAI & own_addr() const;

          bool init(handler_old0 & h, SAI address, int backlog=100);
          bool start();
          bool stop();

          pevent & start_event();
          pevent & exit_event();

          struct impl;
        private:
          /* private implementation */
          std::auto_ptr<impl> impl_;

          /* no-copy */
          lstnr_old0(const lstnr_old0 & other);
          lstnr_old0 & operator=(const lstnr_old0 & other);

          /* for trace and debug */
          CSL_OBJ(csl::comm::tcp,lstnr_old0);
          USE_EXC();
      };
    }
  }
}

#endif /*__cplusplus*/
#endif /*_csl_comm_tcp_lstnr_old0_hh_included_*/
