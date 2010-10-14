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
#endif //0

#include "codesloop/common/libev/evwrap.h"
#include "codesloop/common/logger.hh"
#include "codesloop/comm/exc.hh"
#include "codesloop/comm/evmgr.hh"
#include "codesloop/nthread/mutex.hh"
#include "codesloop/nthread/thread.hh"

namespace csl
{
  using namespace nthread;
  using namespace common;

  namespace comm
  {
    struct evmgr::impl
    {
      int dummy_;
    };

    bool evmgr::init() { return false; /*TODO*/ }
    bool evmgr::shutdown() { return false; /*TODO*/ }

    bool evmgr::add_timer(int tid, uint32_t timeout_ms, handler::flags_t fl, handler & h) { return false; /*TODO*/ }
    bool evmgr::add_fd(int fd, handler::flags_t fl, handler & h) { return false; /*TODO*/ }
    bool evmgr::add_signal(int s, handler::flags_t fl, handler & h) { return false; /*TODO*/ }
    bool evmgr::add_idle(int iid, handler::flags_t fl, handler & h) { return false; /*TODO*/ }

    bool evmgr::remove_timer(int tid) { return false; /*TODO*/ }
    bool evmgr::remove_fd(int fd) { return false; /*TODO*/ }
    bool evmgr::remove_signal(int sig) { return false; /*TODO*/ }
    bool evmgr::remove_idle(int iid) { return false; /*TODO*/ }

    evmgr::evmgr() : impl_(new impl) { /*TODO*/ }
    evmgr::~evmgr() { /*TODO*/ }

    /* no-copy */
    evmgr::evmgr(const evmgr &) : impl_(0) {}
    evmgr & evmgr::operator=(const evmgr &) { return *this; }

  } /* end of ns:comm */
} /* end of ns::csl */

/* EOF */
