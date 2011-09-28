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

#ifndef _csl_common_logger_hh_included_
#define _csl_common_logger_hh_included_
#include "codesloop/common/str.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/excbase.hh"

namespace csl
{
  namespace common
  {
    class logger_base : public stream
    {
    public:
      virtual ~logger_base() {}

    private:
      bufmgr logbufs_;
    };

    class file_logger : public logger_base
    {
    public:
      CSL_CLASS( csl::common::file_logger );
      CSL_DECLARE_EXCEPTION( cannot_open );

      // output interface
      inline stream & push(bufmgr::item p) { return pushref(p); }
      stream & pushref(const bufmgr::item & p);

      file_logger(const char * file_name);
      virtual ~file_logger() {}

      // -- ignored:
      // packet frame
      inline stream & start() { return *this; }
      inline stream & end()   { return *this; }
      inline stream & flush() { return *this; }

      // input interface
      inline bool popref(bufmgr::item & p) { return false; }
      inline size_t poll(size_t sz, uint32_t & timeout_ms) { return 0; }

      // stats
      inline size_t position()  const { return 0; }
      inline size_t available() const { return 0; }

      // events
      inline stream & set_event_cb(event & ev) { return *this; }

    private:
      file_logger(const file_logger&) {}
      file_logger & operator=(const file_logger&) { return *this; }
      str     file_name_;
      FILE *  fp_;
    };

    class logger
    {
    public:
      CSL_CLASS( csl::common::file_logger );
      CSL_DECLARE_EXCEPTION( not_initialized );

      static logger_base * get();
      static void set(logger_base & l);

    private:
      static logger_base * inst_;
    };
  }
}

// --- info/error   : log always
// --- trace        : cumulate+log when needed
// --- scoped       : enter / leave  // only trace ???

#endif /*_csl_common_logger_hh_included_*/
