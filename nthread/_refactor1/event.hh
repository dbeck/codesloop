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

#ifndef _csl_nthread_event_hh_included_
#define _csl_nthread_event_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"

#ifdef __cplusplus
#include <memory>

namespace csl
{
  namespace nthread
  {
    class event
    {
    public:
      event();
      ~event();

      void clear_available();
      bool notify(unsigned int n=1);
      bool notify_all();
      bool wait(unsigned long timeout_ms=0);
      bool wait_nb();
      unsigned int waiting_count();
      unsigned int available_count();

    private:
      struct impl;
      std::auto_ptr<impl> impl_;

      // no-copy
      event(const event & other);
      event & operator=(const event & other);

      CSL_OBJ(csl::nthread, event);
      USE_EXC();
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_nthread_event_hh_included_ */

/* EOF */
