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

#include "codesloop/comm/udp_listener.hh"
#include <assert.h>

using namespace csl::comm;
using namespace csl::common;

namespace test_udp_listener
{
  class handler : public csl::comm::msghandler
  {
  public:
    void operator()(ksmsg m, addr a, int fd)
    {
      try
      {
        scoped_kspin_lock lck(m.get_lock());
        const char * bf = reinterpret_cast<const char *>(m.buffer().buf_);
        std::string s(bf, bf+m.buffer().len_);
        char host[NI_MAXHOST], service[NI_MAXSERV];
        int err = getnameinfo(a.get(),
                        a.len(), host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);

        if(!err)
        {
          printf("Received: %s (from %s:%s) %ld bytes (lockid:%d)\n",
              s.c_str(),
              host,
              service,
              m.buffer().len_,
              m.get_lock().id());
          ::sendto(fd,bf,m.buffer().len_,0,a.get(),a.len());
        }
      }
      catch(const scoped_kspin_lock::key_changed & e)
      {
        e.print();
      }
    }
    handler() {}
  };

  void start_and_stop()
  {
    handler h;
    udp::listener l("localhost","22334",h);
    assert(l.start() == true);
    SleepSeconds(1000);
    assert(l.stop() == true);
  }
}

using namespace test_udp_listener;

int main()
{
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
