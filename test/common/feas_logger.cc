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

#ifndef CSL_INFO_ENABLED
#define CSL_INFO_ENABLED 1
#endif // CSL_INFO_ENABLED

#include "codesloop/common/logger.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/allocator.hh"
#include <vector>

using namespace csl::common;

namespace test_logger
{
  const char * class_name() { return "test_logger"; }

  void loc_baseline()
  {
    CSL_DEFINE_LOGGER_LOCATION(2);
  }

  void info_baseline()
  {
    CSL_INFO( "hello" );
  }

  void msg_baseline()
  {
    lgr::loc * p = 0;
    CSL_DEFINE_LOGGER_LOCATION_PTR(1,p);
    lgr::msg m(*p);
  }

  void part_stdvec()
  {
    std::vector<lgr::msg::part> pv;
    pv.reserve(16);
  }

  void part_stdvec2()
  {
    std::vector<lgr::msg::part> pv(16);
  }

  void part_simpstack()
  {
    std::vector< lgr::msg::part, allocator< lgr::msg::part,16,impl::simpstack > > pv(16);
  }

  void part_simpstack2()
  {
    std::vector< lgr::msg::part, allocator< lgr::msg::part,16,impl::simpstack > > pv;
    pv.reserve(16);
  }

  void print_locs()
  {
    CSL_DEFINE_LOGGER_LOCATION(1);

    uint32_t nlocs = lgr::locs::instance().nlocs();
    for( uint32_t i=0;i<nlocs;++i )
    {
      lgr::loc & l(lgr::locs::instance().get(i));
      printf("%s:%d %s::%s [level=%d] [id=%d] %s\n",
          l.file(),l.line(),l.clazz(),l.func(),l.level(),l.locid(),
          (l.enabled()? "ENABLED":"DISABLED"));
    }
  }

  class dummylog : public logger_base
  {
  public:
    CSL_CLASS( csl::common::dummy );

    inline stream & push(ksmsg p) { return *this; }
    dummylog() {}
    virtual ~dummylog() {}
  };

}

using namespace test_logger;

int main()
{
  dummylog l;
  logger::set(l);

  csl_common_print_results( "loc   baseline", csl_common_test_timer_v0(loc_baseline),"" );
  csl_common_print_results( "info  baseline", csl_common_test_timer_v0(info_baseline),"" );
  csl_common_print_results( "msg   baseline", csl_common_test_timer_v0(msg_baseline),"" );
  csl_common_print_results( "part  stdvec  ", csl_common_test_timer_v0(part_stdvec),"" );
  csl_common_print_results( "part  stdvec2 ", csl_common_test_timer_v0(part_stdvec2),"" );
  csl_common_print_results( "part  sstack  ", csl_common_test_timer_v0(part_simpstack),"" );
  csl_common_print_results( "part  sstack2 ", csl_common_test_timer_v0(part_simpstack2),"" );

  print_locs();
  return 0;
}

// EOF
