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

#ifndef _csl_common_excbase_hh_included_
#define _csl_common_excbase_hh_included_
#include "codesloop/common/common.h"

namespace csl
{
  namespace common
  {
    class excbase
    {
    private:
      const char *  file_;
      unsigned int  line_;
      const char *  func_;
      const char *  reason_;
      const char *  class_;
      
      excbase() {}

    public:
      excbase( const char * fil,
               unsigned int lin,
               const char * fun,
               const char * reas,
               const char * cls
            )
        : file_(fil),
          line_(lin),
          func_(fun),
          reason_(reas),
          class_(cls) {}
        
      virtual ~excbase() {}
      
      const char * file()   const  { return file_;   }
      unsigned int line()   const  { return line_;   }
      const char * func()   const  { return func_;   }
      const char * reason() const  { return reason_; }
      const char * clazz()  const  { return class_;  }

      virtual void print() const
      {
        printf(
            "Exception details:\n"
            "File: %s\n"
            "Line: %d\n"
            "Function: %s\n"
            "Reason: %s\n"
            "Class: %s\n",
            file_,
            line_,
            func_,
            reason_,
            class_);
      }
    };
    
#ifndef CSL_DECLARE_EXCEPTION
#define CSL_DECLARE_EXCEPTION(NAME)                          \
    class NAME : public csl::common::excbase {               \
    public:                                                  \
      NAME( const char * fil,                                \
            unsigned int lin,                                \
            const char * fun,                                \
            const char * reas,                               \
            const char * cls ) :                             \
            csl::common::excbase(fil,lin,fun,reas,cls) {}    \
      static const char * name() { return (#NAME); }         \
      virtual ~ NAME() {}                                    \
    };
#endif /*CSL_DECLARE_EXCEPTION*/

#ifndef CSL_DECLARE_DERIVED_EXCEPTION
#define CSL_DECLARE_DERIVED_EXCEPTION(NAME,BASE)             \
    class NAME : public BASE {                               \
    public:                                                  \
      NAME( const char * fil,                                \
            unsigned int lin,                                \
            const char * fun,                                \
            const char * reas,                               \
            const char * cls ) :                             \
            BASE(fil,lin,fun,reas,cls) {}                    \
      static const char * name() { return (#NAME); }         \
      virtual ~ NAME() {}                                    \
    };
#endif /*CSL_DECLARE_DERIVED_EXCEPTION*/

#ifndef CSL_THROW
#define CSL_THROW(NAME) { throw NAME(__FILE__,__LINE__,__func__,NAME :: name(), class_name()); }
#endif /*CSL_THROW*/
  }
}

#endif /*_csl_common_excbase_hh_included_*/
