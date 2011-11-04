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
#include "codesloop/common/lgr_loc.hh"
#include "codesloop/common/lgr_msg.hh"

namespace csl
{
  namespace common
  {
    class logger_base : public stream
    {
    public:
      virtual ~logger_base() {}

      enum { info_=1, error_=2, trace_=3, scoped_=4 };

      virtual lgr::msg construct_msg(lgr::loc & l) { return lgr::msg(l); }

    public: // -- ignored:
      // input interface
      inline bool pop(ksmsg & p) { return false; }
      inline size_t poll(size_t sz, uint32_t & timeout_ms) { return 0; }
      // events
      inline stream & set_event_cb(event & ev) { return *this; }
      // packet frame
      inline stream & start() { return *this; }
      inline stream & end()   { return *this; }
      inline stream & flush() { return *this; }
    };

    class logger
    {
    public:
      CSL_CLASS( csl::common::file_logger );
      CSL_DECLARE_EXCEPTION( not_initialized );

      static logger_base & get();
      static void set(logger_base & l);

    private:
      static logger_base * inst_;
    };
  }
}

// --- info/error   : log always
// --- trace        : cumulate+log when needed
// --- scoped       : enter / leave : only trace

// INFO(val << val << val);
// ERROR(val << val << val);
// TRACE(val << val << val);
// SCOPED(val << val << val << val);

#ifndef CSL_LOGGER_COMMON_
#define CSL_LOGGER_COMMON_( LEVEL, EXPR ) \
  do { \
    csl::common::lgr::loc * __loc_ptr__ = 0; \
    CSL_DEFINE_LOGGER_LOCATION_PTR( csl::common::logger_base:: LEVEL, __loc_ptr__ ); \
    if( __loc_ptr__->enabled() ) \
      logger::get().construct_msg(*__loc_ptr__) << EXPR; \
  } while(0)
#endif // CSL_LOGGER_COMMON_

#ifdef CSL_INFO_ENABLED
#define CSL_INFO( EXPR ) CSL_LOGGER_COMMON_( info_, EXPR )
#else // !CSL_INFO_ENABLED
#define CSL_INFO( EXPR )
#endif

#ifdef CSL_ERROR_ENABLED
#define CSL_ERROR( EXPR ) CSL_LOGGER_COMMON_( error_, EXPR )
#else // !CSL_ERROR_ENABLED
#define CSL_ERROR( EXPR )
#endif

#ifdef CSL_TRACE_ENABLED
#define CSL_TRACE( EXPR ) CSL_LOGGER_COMMON_( trace_, EXPR )
#else // !CSL_TRACE_ENABLED
#define CSL_TRACE( EXPR )
#endif

#ifdef CSL_SCOPED_TRACE_ENABLED
#define CSL_SCOPED( EXPR ) \
  lgr::loc * __scoped_loc_ptr__ = 0; \
  CSL_DEFINE_LOGGER_LOCATION_PTR( csl::common::logger_base::scoped_, __loc_ptr__ ); \
  logger_base::msg __scoped_logger__(logger::get().construct_msg(*__scoped_loc_ptr__)); \
  __scoped_logger__ << EXPR;
#else // !CSL_SCOPED+TRACE_ENABLED
#define CSL_SCOPED( EXPR )
#endif

#endif /*_csl_common_logger_hh_included_*/
