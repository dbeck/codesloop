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
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class dbcexc
    {
    private:
      const char *  file_;
      unsigned int  line_;
      const char *  func_;
      const char *  expr_;
      const char *  dbctype_;
      
      dbcexc() {}

    public:
      dbcexc( const char * file,
              unsigned int line,
              const char * func,
              const char * expr,
              const char * dbctype
            )
        : file_(file),
          line_(line),
          func_(func),
          expr_(expr),
          dbctype_(dbctype) {}
        
      ~dbcexc() {}
    };
  }
}

#ifndef CSL_ASSERT_THROW
# ifdef DEBUG
#  define CSL_ASSERT_THROW(EXPR,TYPE) if( !(EXPR) ) throw dbcexc(__FILE__,__LINE__,__func__,#EXPR,TYPE);
# else /*DEBUG*/
#  define CSL_ASSERT_THROW(EXPR,TYPE)
# endif /*DEBUG*/
#endif /*CSL_ASSERT_THROW*/


#define CSL_REQUIRE(EXPR) CSL_ASSERT_THROW( (EXPR),"REQUIRE" );
#define CSL_ENSURE(EXPR)  CSL_ASSERT_THROW( (EXPR),"ENSURE" );
#define CSL_CHECK_INVARIANT() CSL_ASSERT_THROW( (this->csl_invariant()),"INVARIANT" );

#ifndef CSL_INVARIANT
# ifdef DEBUG
#  define CSL_INVARIANT() bool csl_invariant() const
# else /*DEBUG*/
#  define CSL_INVARIANT()
# endif /*DEBUG*/
#endif /*CSL_INVARIANT*/

#ifndef CSL_VIRTUAL_INVARIANT
# ifdef DEBUG
#  define CSL_VIRTUAL_INVARIANT() virtual bool csl_invariant() const
# else /*DEBUG*/
#  define CSL_VIRTUAL_INVARIANT()
# endif /*DEBUG*/
#endif /*CSL_VIRTUAL_INVARIANT*/

#endif /*__cplusplus*/
#endif /*_csl_common_dbc_hh_included_*/

