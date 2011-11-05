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

#ifndef CSL_SCOPED_TRACE_ENABLED
#define CSL_SCOPED_TRACE_ENABLED 1
#endif // CSL_SCOPED_TRACE_ENABLED

#ifndef CSL_LOGGER_LOCATION_SETTING_ENABLED
#define CSL_LOGGER_LOCATION_SETTING_ENABLED 1
#endif // CSL_LOGGER_LOCATION_SETTING_ENABLED

#include "codesloop/common/common.h"
#include "codesloop/common/logger.hh"
#include "codesloop/common/test_timer.h"
#include <string>

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
    CSL_INFO( "hello info" );
  }

  void info_multi()
  {
    int32_t i(123);
    double d(678.123);
    CSL_INFO( "integer value: " << i << " double xxx:" << d << " hello world" );
  }

  void scoped_baseline()
  {
    CSL_SCOPED( "scoped hello" );
  }

  void scoped_multi()
  {
    CSL_SCOPED( "check scoped trace" );
    int32_t i(123);
    double d(678.123);
    CSL_INFO( "integer value: " << i << " double xxx:" << d << " hello world" );
  }

  void msg_baseline()
  {
    lgr::loc * p = 0;
    CSL_DEFINE_LOGGER_LOCATION_PTR(1,p);
    lgr::msg m(*p,logger::get());
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

  class simplelog : public logger_base
  {
  public:
    CSL_CLASS( test_logger::simplelog );

    template <typename T> T get(uint8_t * p, size_t & pos)
    {
      T ret = 0;
      ::memcpy(&ret,p+pos,sizeof(T));
      pos += sizeof(T);
      return ret;
    }

    inline stream & push(ksmsg p)
    {
      try
      {
        size_t pos = 0;
        scoped_kspin_lock lck(p.get_lock());
        unsigned int locid = get<unsigned int>(p.buffer().buf_,pos);
        lgr::loc & l(lgr::locs::instance().get(locid));
        printf("LOG FROM: %s:%d %s::%s [level=%d] [id=%d] %s  <%ld bytes>\n",
            l.file(),l.line(),l.clazz(),l.func(),l.level(),l.locid(),
            (l.enabled()? "ENABLED":"DISABLED"),
            p.buffer().len_);

        while(pos < p.buffer().len_)
        {
          unsigned char tp = get<unsigned char>(p.buffer().buf_,pos);
          switch(tp)
          {
          case lgr::type_select<lgr::tag>::val_:
            printf("  -- tag:\n");
            break;

          case lgr::type_select<lgr::name>::val_:
            printf("  -- name:\n");
            break;

          case lgr::type_select<const char *>::val_:
          {
            unsigned short len = get<unsigned short>(p.buffer().buf_,pos);
            const char * px = reinterpret_cast<const char *>(p.buffer().buf_+pos);
            std::string s(px,px+len);
            pos += len;
            printf("  -- string: '%s' len:%d\n",s.c_str(),len);
            break;
          }

          case lgr::type_select<int32_t>::val_:
          {
            int32_t v = get<int32_t>(p.buffer().buf_,pos);
            printf("  -- int32 %d:\n",v);
            break;
          }

          case lgr::type_select<double>::val_:
          {
            double v = get<double>(p.buffer().buf_,pos);
            printf("  -- double %f:\n",v);
            break;
          }

          case lgr::type_select<lgr::end_of_record>::val_:
            printf("  -- end_of_rec\n");
            break;

          case lgr::type_select<lgr::return_from_function>::val_:
            printf("  -- return\n");
            break;

          default:
            break;
          };
        };
      }
      catch(const scoped_kspin_lock::key_changed & e)
      {
        e.print();
      }
      catch(const excbase & e)
      {
        e.print();
      }

      return *this;
    }
    simplelog() : logger_base() {}
    virtual ~simplelog() {}

  private:

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
  dummylog l;
  logger::set(l);

  csl_common_print_results( "loc     baseline", csl_common_test_timer_v0(loc_baseline),"" );
  csl_common_print_results( "info    baseline", csl_common_test_timer_v0(info_baseline),"" );
  csl_common_print_results( "info       multi", csl_common_test_timer_v0(info_multi),"" );
  csl_common_print_results( "scoped  baseline", csl_common_test_timer_v0(scoped_baseline),"" );
  csl_common_print_results( "scoped     multi", csl_common_test_timer_v0(scoped_multi),"" );
  csl_common_print_results( "msg     baseline", csl_common_test_timer_v0(msg_baseline),"" );

  print_locs();

  simplelog s;
  logger::set(s);
  info_baseline();
  scoped_baseline();
  info_multi();
  scoped_multi();

  disable_all();

  csl_common_print_results( "loc     baseline", csl_common_test_timer_v0(loc_baseline),"" );
  csl_common_print_results( "info    baseline", csl_common_test_timer_v0(info_baseline),"" );
  csl_common_print_results( "info       multi", csl_common_test_timer_v0(info_multi),"" );
  csl_common_print_results( "scoped  baseline", csl_common_test_timer_v0(scoped_baseline),"" );
  csl_common_print_results( "scoped     multi", csl_common_test_timer_v0(scoped_multi),"" );
  csl_common_print_results( "msg     baseline", csl_common_test_timer_v0(msg_baseline),"" );

  enable_2nd();

  info_baseline();
  scoped_baseline();
  info_multi();
  scoped_multi();

  print_locs();

  return 0;
}

// EOF
