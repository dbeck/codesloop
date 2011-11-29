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

#include "codesloop/comm/tcp_listener.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/lgr_stderr_logger.hh"
#include <assert.h>

using namespace csl::comm;
using namespace csl::common;

namespace test_tcp_listener
{
  class handler : public csl::comm::fdhandler
  {
  public:
    void operator()(autofd fd, addr a)
    {
      CSL_INFO(L"New connection on fd:" << fd.get() );
    }
  };

  void start_and_stop()
  {
    handler h;
    tcp::listener l("localhost","22334",h);
    assert(l.start() == true);
    SleepSeconds(1000);
    assert(l.stop() == true);
  }
}

using namespace test_tcp_listener;

int main()
{
  csl::common::lgr::stderr_logger errlog;
  csl::common::logger::set(errlog);

  try
  {
    start_and_stop();
  }
  catch(const excbase & e)
  {
    e.print();
  }
  return 0;
}

// EOF
