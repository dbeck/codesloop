/*
Copyright (c) 2008,2009, David Beck, Tamas Foldi
 
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
 
#ifndef _csl_rpc_csrgen_hh_included_
#define _csl_rpc_csrgen_hh_included_

#include "common.h"
#ifdef __cplusplus
#include "obj.hh"

namespace csl 
{ 
  namespace rpc 
  {
  
    enum token_type {
      TT_UNKNOWN    = 0,
      TT_VERSION    = 1,
      TT_NAME       = 2,
      TT_NAMESPACE  = 3,
      TT_INCLUDE    = 4,
      TT_FUNCTION   = 5,
      TT_DISPOSABLE_FUNCTION  = 6,
      TT_FUNCTION_END = 7,
      TT_PARAM_MOD  = 8,
      TT_PARAM_TYPE = 9,
      TT_PARAM_NAME = 10,
      TT_COMMENT    = 11,
      TT_LAST       = 12
    };
    extern const char * token_type_name[];

    enum param_kind {
      MD_INPUT      = 0,
      MD_OUTPUT     = 1,
      MD_INOUT      = 2,
      MD_EXCEPTION  = 3
    };
    extern const char * param_kind_name[];

    struct token_info
    {
      char * p;         ///< actual buffer pointer
      char * pe;        ///< eof point
      char * ls;        ///< line start
      char * ts;        ///< token start
      int cs;           ///< current char
      int curline;      ///< current line in file
      token_type type;  ///< token identifier
      param_kind modifier;  ///< input/output/exc
      int array_length; ///< array definition: 0 - not an array, -1 undefined size, n = n sized arry
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_rpc_csrgen_hh_included_ */
