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

#include "codesloop/common/logger.hh"
#include "codesloop/common/lgr_silent_logger.hh"

namespace csl
{
  namespace common
  {
    logger_base & logger::get()
    {
      if( !inst_ ) { CSL_THROW(not_initialized); }
      return *inst_;
    }
    void logger::set(logger_base & l) { inst_ = &l; }

    namespace lgr
    {
      silent_logger silent_logger::instance_;
    }

    logger_base * logger::inst_ = &(lgr::silent_logger::instance());

    const char * logger_base::level_to_string(unsigned int level)
    {
      static const char * strs[] = { "INF", "ERR", "TRC", "SCD" };
      return strs[level%4];
    }

    const wchar_t * logger_base::level_to_wstring(unsigned int level)
    {
      static const wchar_t * wstrs[] = { L"INF", L"ERR", L"TRC", L"SCD" };
      return wstrs[level%4];
    }
  }
}
