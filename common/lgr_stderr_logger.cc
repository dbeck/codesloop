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

#include "codesloop/common/lgr_stderr_logger.hh"
#include "codesloop/common/lgr_msg.hh"
#include "codesloop/common/str.hh"

namespace csl
{
  namespace common
  {
    namespace lgr
    {
      stream & stderr_logger::push(ksmsg p)
      {
        try
        {
          scoped_kspin_lock lck(p.get_lock());
          str result;
          lgr::msg::to_str(p.buffer().buf_, p.buffer().len_, result);
          if( !result.empty() )
          {
            fwrite(result.c_str(),sizeof(wchar_t),result.nchars(),stderr);
            fflush(stderr);
          }
        }
        catch(const scoped_kspin_lock::key_changed & e)
        {
          // XXX TODO
        }
        catch(const excbase & e)
        {
          // XXX TODO
        }
        return *this;
      }
    }
  }
}
