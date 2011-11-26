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

#ifndef _csl_common_lgr_msg_hh_included_
#define _csl_common_lgr_msg_hh_included_
#include "codesloop/common/lgr_loc.hh"
#include "codesloop/common/lgr_msg_types.hh"
#include "codesloop/common/ksbuf.hh"
#include <vector>

#ifndef CSL_LOGGER_MAX_LOG_LENGTH
#define CSL_LOGGER_MAX_LOG_LENGTH 2048
#endif // CSL_LOGGER_MAX_LOG_LENGTH

#ifndef CSL_LOGGER_MAX_MESSAGE_COUNT // in memory
#define CSL_LOGGER_MAX_MESSAGE_COUNT 4096
#endif // CSL_LOGGER_MAX_MESSAGE_COUNT

namespace csl
{
  namespace common
  {
    class logger_base;
    class str;

    namespace lgr // logger helpers
    {
      class msg
      {
      public:
        CSL_CLASS( csl::common::lgr::msg );
        CSL_DECLARE_EXCEPTION( lock_failure );

        typedef csl::common::ksbuf<CSL_LOGGER_MAX_MESSAGE_COUNT, CSL_LOGGER_MAX_LOG_LENGTH> buffer_t;
        typedef buffer_t::result msgdata_t;

        msg(lgr::loc &, logger_base &);
        inline msg(lgr::loc &, logger_base &, const disabled_log &) : loc_(0), logger_(0) {}
        ~msg();

        msg & operator<<(const char *);
        msg & operator<<(const wchar_t *);
        msg & operator<<(const csl::common::str & s);
        inline msg & operator<<(const tag &)  { append<uint8_t>(type_select<tag>::sel_);  return *this; }
        inline msg & operator<<(const name &) { append<uint8_t>(type_select<name>::sel_); return *this; }
        inline msg & operator<<(const end_of_record &)
        { append<uint8_t>(type_select<end_of_record>::sel_); flush(); eor_seen_ = true; return *this; }

      private:
        inline msg & operator<<(const return_from_function &)
        { append<uint8_t>(type_select<return_from_function>::sel_); flush(); return *this; }

      public:
        // catch all: treat them as integral types
        template <typename T> inline msg & operator<<(const T & v)
        { append(type_select<T>::sel_,&v,sizeof(T)); return *this; }

        static void to_str(const uint8_t * encoded, size_t len, str & result);

      private:
        // internal functions
        void flush();
        void append(const void * data, size_t len);

        template<typename T>
        inline void append(const T & t) { append(&t,sizeof(T)); }

        inline void append(unsigned char typ,const void * data, size_t len)
        { append<uint8_t>(typ); append(data,len); }

        lgr::loc *              loc_;
        logger_base *           logger_;
        msgdata_t               data_;
        oneshot_kspin_lock      lock_;
        size_t                  used_;
        bool                    eor_seen_;
        bool                    skip_;
      };
    }
  }
}

#endif //_csl_common_lgr_msg_hh_included_
