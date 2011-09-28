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

#ifndef _csl_common_dbc_hh_included_
#define _csl_common_dbc_hh_included_
#include "codesloop/common/common.h"
#include "codesloop/common/excbase.hh"

namespace csl
{
  namespace common
  {
    class dbcexc : public csl::common::excbase
    {
    private:
      const char *  expr_;
      const char *  dbctype_;
      
      dbcexc() :
        csl::common::excbase(__FILE__,__LINE__,__func__,"",""),
        expr_(""),
        dbctype_("") {}

    public:
      dbcexc( const char * fil,
              unsigned int lin,
              const char * fun,
              const char * reas,
              const char * cls,
              const char * exp,
              const char * dbctyp
            ) : 
        csl::common::excbase(fil,lin,fun,reas,cls),
        expr_(exp),
        dbctype_(dbctyp) {}
        
      virtual ~dbcexc() {}
      
      const char * expr()    const { return expr_;    }
      const char * dbctype() const { return dbctype_; }
    };
  }
}

#ifndef CSL_ASSERT_THROW
# ifdef CSL_DEBUG
#  define CSL_ASSERT_THROW(EXPR,TYPE)                        \
      if( !(EXPR) )                                          \
      {                                                      \
        throw csl::common::dbcexc( __FILE__,                 \
                      __LINE__,                              \
                      __func__,                              \
                      "DBC {" TYPE "} failed: " #EXPR,       \
                      class_name(),                          \
                      #EXPR,                                 \
                      TYPE);                                 \
      }
# else /*CSL_DEBUG*/
#  define CSL_ASSERT_THROW(EXPR,TYPE)
# endif /*CSL_DEBUG*/
#endif /*CSL_ASSERT_THROW*/

#ifndef CSL_DEBUG_ASSIGN
# ifdef CSL_DEBUG
#  define CSL_DEBUG_ASSIGN(A,B) (A)=(B)
# else
#  define CSL_DEBUG_ASSIGN(A,B)
# endif /*CSL_DEBUG*/
#endif /*CSL_DEBUG_ASSIGN*/

#define CSL_REQUIRE(EXPR) CSL_ASSERT_THROW( (EXPR),"REQUIRE" );
#define CSL_ENSURE(EXPR)  CSL_ASSERT_THROW( (EXPR),"ENSURE" );
#define CSL_CHECK_INVARIANT() CSL_ASSERT_THROW( (this->csl_invariant()),"INVARIANT" );

#ifndef CSL_INVARIANT
# ifdef CSL_DEBUG
#  define CSL_INVARIANT() bool csl_invariant() const
# else /*CSL_DEBUG*/
#  define CSL_INVARIANT()
# endif /*CSL_DEBUG*/
#endif /*CSL_INVARIANT*/

#ifndef CSL_VIRTUAL_INVARIANT
# ifdef CSL_DEBUG
#  define CSL_VIRTUAL_INVARIANT() virtual bool csl_invariant() const
# else /*CSL_DEBUG*/
#  define CSL_VIRTUAL_INVARIANT()
# endif /*CSL_DEBUG*/
#endif /*CSL_VIRTUAL_INVARIANT*/

#endif /*_csl_common_dbc_hh_included_*/

