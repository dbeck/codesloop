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

#ifndef _csl_comm_evmgr_hh_included_
#define _csl_comm_evmgr_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#ifdef __cplusplus
#include <memory>

namespace csl
{
  namespace comm
  {
    class evmgr
    {
      public:
        class handler
        {
          public:
            typedef uint32_t flags_t;

            static const flags_t ok_reply_       = 0;
            static const flags_t remove_reply_   = 1<<0;
            static const flags_t error_reply_    = 1<<1;
            static const flags_t start_event_    = 1<<2;
            static const flags_t stop_event_     = 1<<3;
            static const flags_t read_event_     = 1<<4;
            static const flags_t write_event_    = 1<<5;
            static const flags_t timer_event_    = 1<<6;
            static const flags_t stat_event_     = 1<<7;
            static const flags_t signal_event_   = 1<<8;
            static const flags_t idle_event_     = 1<<9;
            static const flags_t error_event_    = 1<<10;

            virtual flags_t on_timer(int tid, flags_t evfl)  { return ok_reply_; }
            virtual flags_t on_fd(int fd, flags_t evfl)      { return ok_reply_; }
            virtual flags_t on_signal(int sig, flags_t evfl) { return ok_reply_; }
            virtual flags_t on_idle(int iid, flags_t evfl)   { return ok_reply_; }

            virtual ~handler() {}
        };

        bool init();
        bool shutdown();

        /* timer support */
        bool add_timer( int tid, uint32_t timeout_ms, handler::flags_t fl, handler & h );
        bool remove_timer( int tid );

        /* fd support */
        bool add_fd( int fd, handler::flags_t fl, handler & h );
        bool remove_fd( int fd );

        /* signal support */
        bool add_signal( int sig, handler::flags_t fl, handler & h );

        bool remove_signal( int sig );

        /* idle support */
        bool add_idle( int iid, handler::flags_t fl, handler & h );
        bool remove_idle( int iid );

        evmgr();
        virtual ~evmgr();

      private:
        struct impl;

        /* private implementation */
        std::auto_ptr<impl> impl_;

        /* no-copy */
        evmgr(const evmgr & other);
        evmgr & operator=(const evmgr & other);

        /* for trace and debug */
        CSL_OBJ(csl::comm,evmgr);
        USE_EXC();
    };
  }
}

#endif /*__cplusplus*/
#endif /*_csl_comm_evmgr_hh_included_*/
