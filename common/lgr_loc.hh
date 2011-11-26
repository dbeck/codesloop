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

#ifndef _csl_common_lgr_loc_hh_included_
#define _csl_common_lgr_loc_hh_included_
#include "codesloop/common/dbc.hh"
#include <atomic>

#ifndef CSL_COMMON_MAX_LOCS
#define CSL_COMMON_MAX_LOCS 100000
#endif //CSL_COMMON_MAX_LOCS

namespace csl
{
  namespace common
  {
    namespace lgr // logger helpers
    {
      class loc;

      class locs
      {
      public:
        CSL_CLASS( csl::common::lgr::locs );
        CSL_DECLARE_EXCEPTION( out_of_range );

        static inline locs & instance() { return instance_; }

        inline uint32_t reg(loc & l)
        {
          uint32_t ret = nlocs_++;
          CSL_REQUIRE( ret < CSL_COMMON_MAX_LOCS );
          locs_[ret] = &l;
          return ret;
        }

        inline loc & get(uint32_t id)
        {
          CSL_REQUIRE( id <= nlocs_.load() );
          return *(locs_[id]);
        }

        inline uint32_t nlocs() const { return nlocs_.load(); }

      private:
        typedef loc* locptr_t;
        typedef std::atomic_uint_fast32_t counter_t;

        locs() : nlocs_(0) {}

        static locs instance_;
        locptr_t    locs_[CSL_COMMON_MAX_LOCS];
        counter_t   nlocs_;

        locs(const locs&) = delete;
        locs& operator=(const locs &) = delete;
      };

      class loc
      {
      public:
        loc(const char * fl,
            const wchar_t * wfl,
            unsigned int ln,
            const char * fu,
            const char * cs,
            const wchar_t * wcs,
            unsigned int ll,
            locs & lcs,
            uint32_t enab=1)
            :
              file_(fl),
              wfile_(wfl),
              line_(ln),
              func_(fu),
              clazz_(cs),
              wclazz_(wcs),
              level_(ll),
              enabled_(enab)
        {
          locid_ = lcs.reg(*this);
        }

        inline const char * file()      const { return file_;   }
        inline const wchar_t * wfile()  const { return wfile_;  }
        inline unsigned int line()      const { return line_;   }
        inline const char * func()      const { return func_;   }
        inline const char * clazz()     const { return clazz_;  }
        inline const wchar_t * wclazz() const { return wclazz_; }
        inline unsigned int level()     const { return level_;  }
        inline unsigned int locid()     const { return locid_;  }

        inline bool enabled() const { return (enabled_.load() == 1); }
        inline void disable() { enabled_ = 0; }
        inline void enable()  { enabled_ = 1; }

      private:
        const char *            file_;
        const wchar_t *         wfile_;
        unsigned int            line_;
        const char *            func_;
        const char *            clazz_;
        const wchar_t *         wclazz_;
        unsigned int            level_;
        unsigned int            locid_;
        std::atomic_uint        enabled_;

        loc() = delete;
        loc(const loc&) = delete;
        loc& operator=(const loc &) = delete;
      };
    }
  }
}

#ifndef CSL_DEFINE_LOGGER_LOCATION
#define CSL_DEFINE_LOGGER_LOCATION(LEVEL) \
    do { \
      static csl::common::lgr::loc __logger__location__(        \
          __FILE__, L""__FILE__,                                \
          __LINE__,                                             \
          __func__,                                             \
          class_name(), wclass_name(),                          \
          LEVEL,                                                \
          csl::common::lgr::locs::instance());                  \
    } while(0)
#endif //CSL_DEFINE_LOGGER_LOCATION

#ifndef CSL_DEFINE_LOGGER_LOCATION_PTR
#define CSL_DEFINE_LOGGER_LOCATION_PTR(LEVEL,PTR)               \
    do {                                                        \
      static csl::common::lgr::loc __logger__location__(        \
          __FILE__, L""__FILE__,                                \
          __LINE__,                                             \
          __func__,                                             \
          class_name(), wclass_name(),                          \
          LEVEL,                                                \
          csl::common::lgr::locs::instance());                  \
      PTR = &__logger__location__;                              \
    } while(0)
#endif //CSL_DEFINE_LOGGER_LOCATION_PTR

#ifndef CSL_DEFINE_LOGGER_LOCATION_PTR_INIT
#define CSL_DEFINE_LOGGER_LOCATION_PTR_INIT(LEVEL,PTR,INIT)     \
    do {                                                        \
      static csl::common::lgr::loc __logger__location__(        \
          __FILE__, L""__FILE__,                                \
          __LINE__,                                             \
          __func__,                                             \
          class_name(), wclass_name(),                          \
          LEVEL,                                                \
          csl::common::lgr::locs::instance(),                   \
          INIT);                                                \
      PTR = &__logger__location__;                              \
    } while(0)
#endif //CSL_DEFINE_LOGGER_LOCATION_PTR_INIT


#endif /*_csl_common_lgr_loc_hh_included_*/
