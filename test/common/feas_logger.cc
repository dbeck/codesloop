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

#ifdef CSL_SET_TRACE_DISABLED_BY_DEFAULT
#undef CSL_SET_TRACE_DISABLED_BY_DEFAULT
#endif // CSL_SET_TRACE_DISABLED_BY_DEFAULT

#ifndef CSL_SCOPED_TRACE_ENABLED
#define CSL_SCOPED_TRACE_ENABLED 1
#endif // CSL_SCOPED_TRACE_ENABLED

#ifndef CSL_LOGGER_LOCATION_SETTING_ENABLED
#define CSL_LOGGER_LOCATION_SETTING_ENABLED 1
#endif // CSL_LOGGER_LOCATION_SETTING_ENABLED

#include "codesloop/common/common.h"
#include "codesloop/common/logger.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/lgr_stderr_logger.hh"
#include "codesloop/common/lgr_silent_logger.hh"
#include <string>
#include <iosfwd>
#include <sstream>

using namespace csl::common;

namespace test_logger
{
  CSL_CLASS( test_logger );

  void loc_baseline()
  {
    CSL_DEFINE_LOGGER_LOCATION(2);
  }

  void info_baseline()
  {
    CSL_INFO( "hello info" );
  }

  void info_multi()
  {
    int32_t i(123);
    double d(678.123);
    CSL_INFO( "integer value: " << i << " double xxx:" << d << " hello world" );
  }

  void strstr_baseline()
  {
    std::ostringstream s;
  }

  void strstr_multi()
  {
    std::ostringstream s;
    int32_t i(123);
    double d(678.123);
    s << "integer value: " << i << " double xxx:" << d << " hello world";
  }

  void scoped_baseline()
  {
    CSL_SCOPED( "scoped hello" );
  }

  void scoped_multi()
  {
    CSL_SCOPED( L"check scoped trace" );
    int32_t i(123);
    double d(678.123);
    CSL_INFO( "integer value: " << i << " double xxx:" << d << " hello world" );
  }

  void msg_baseline()
  {
    lgr::loc * p = 0;
    CSL_DEFINE_LOGGER_LOCATION_PTR(1,p);
    lgr::msg m(*p,logger::get());
    m << csl::common::lgr::end_of_record();
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
    CSL_CLASS( test_logger::dummylog );

    inline stream & push(ksmsg p) { return *this; }
    dummylog() : logger_base() {}
    virtual ~dummylog() {}
  };


  void disable_all()
  {
    CSL_DEFINE_LOGGER_LOCATION(1);

    uint32_t nlocs = lgr::locs::instance().nlocs();
    for( uint32_t i=0;i<nlocs;++i )
    {
      lgr::loc & l(lgr::locs::instance().get(i));
      l.disable();
    }
  }

  void enable_2nd()
  {
    CSL_DEFINE_LOGGER_LOCATION(1);

    uint32_t nlocs = lgr::locs::instance().nlocs();
    for( uint32_t i=0;i<nlocs;++i )
    {
      lgr::loc & l(lgr::locs::instance().get(i));
      if( i%2 == 0 ) l.enable();
    }
  }

}

using namespace test_logger;

int main()
{
  lgr::stderr_logger errlog;
  logger::set(errlog);
  scoped_multi();

  lgr::silent_logger silent;
  logger::set(silent);

  csl_common_print_results( "loc     baseline", csl_common_test_timer_v0(loc_baseline),"" );
  csl_common_print_results( "msg     baseline", csl_common_test_timer_v0(msg_baseline),"" );
  csl_common_print_results( "strstr  baseline", csl_common_test_timer_v0(strstr_baseline),"" );
  csl_common_print_results( "strstr     multi", csl_common_test_timer_v0(strstr_multi),"" );
  csl_common_print_results( "info    baseline", csl_common_test_timer_v0(info_baseline),"" );
  csl_common_print_results( "info       multi", csl_common_test_timer_v0(info_multi),"" );
  csl_common_print_results( "scoped  baseline", csl_common_test_timer_v0(scoped_baseline),"" );
  csl_common_print_results( "scoped     multi", csl_common_test_timer_v0(scoped_multi),"" );

  print_locs();

  logger::set(errlog);
  info_baseline();
  scoped_baseline();
  info_multi();
  scoped_multi();

  disable_all();
  logger::set(silent);

  csl_common_print_results( "loc     baseline", csl_common_test_timer_v0(loc_baseline),"" );
  csl_common_print_results( "msg     baseline", csl_common_test_timer_v0(msg_baseline),"" );
  csl_common_print_results( "info    baseline", csl_common_test_timer_v0(info_baseline),"" );
  csl_common_print_results( "info       multi", csl_common_test_timer_v0(info_multi),"" );
  csl_common_print_results( "scoped  baseline", csl_common_test_timer_v0(scoped_baseline),"" );
  csl_common_print_results( "scoped     multi", csl_common_test_timer_v0(scoped_multi),"" );

  enable_2nd();
  logger::set(errlog);

  info_baseline();
  scoped_baseline();
  info_multi();
  scoped_multi();

  print_locs();

  return 0;
}

// EOF
