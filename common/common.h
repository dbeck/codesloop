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

#ifndef _csl_common_common_h_included_
#define _csl_common_common_h_included_

/* for: size_t, NULL */
#include <stddef.h>

#ifndef CSL_CDECL
# ifndef __cplusplus
#  define CSL_CDECL
#  define CSL_OPAQ_TYPE(A) void
# else
#  define CSL_CDECL extern "C"
#  define CSL_OPAQ_TYPE(A) A
# endif
#endif /* CSL_CDECL */

#ifndef PARAM_NOT_NULL
# define PARAM_NOT_NULL(P) \
    if( (P) == NULL ) return -1
#endif

#ifndef PARAM_NOT_ZERO
# define PARAM_NOT_ZERO(P) \
    if( (P) == 0 ) return -1
#endif

#ifndef PARAM_NOT_NULL2
# define PARAM_NOT_NULL2(P,R) \
    if( (P) == NULL ) return (R)
#endif

#ifndef PARAM_NOT_ZERO2
# define PARAM_NOT_ZERO2(P,R) \
    if( (P) == 0 ) return (R)
#endif

#ifdef WIN32
# include "common_win32.h"
#else 
# include "common_unix.h"
#endif

#ifndef cast32
#ifdef __cplusplus
#define cast32(x) (static_cast<uint32_t>(x))
#define cast64(x) (static_cast<uint64_t>(x))
#else
#define cast32(x) ((uint32_t)(x))
#define cast64(x) ((uint64_t)(x))
#endif /*__cplusplus*/
#endif /*cast32 */

#ifndef htonll
#ifdef CSL_BIG_ENDIAN
#define htonll(x)   (x)
#define ntohll(x)   (x)
#else
#ifndef CSL_LITTLE_ENDIAN
#warning "Endian macros not set!!! Fix cmake environment!!!"
#endif
#define htonll_low32(x)  htonl(cast32((cast64(x)&0xFFFFFFFFULL)))
#define htonll_high32(x) htonl(cast32((cast64(x)>>32)))
#define htonll(x) (cast64(htonll_low32(x))<<32 | cast64(htonll_high32(x)))
#define ntohll(x) htonll(x)
#endif
#endif

#ifndef CSL_DEBUG_ASSERT
# ifdef CSL_DEBUG
#  include <assert.h>
#  define CSL_DEBUG_ASSERT(EXPR) assert( (EXPR) );
# else
#  define CSL_DEBUG_ASSERT(EXPR)
# endif /*CSL_DEBUG*/
#endif /*CSL_DEBUG_ASSERT*/

#ifndef CSL_STDIO_H_INCLUDED
# define CSL_STDIO_H_INCLUDED
# include <stdio.h>
#endif /*CSL_STDIO_H_INCLUDED*/

#ifndef CSL_STDLIB_H_INCLUDED
# define CSL_STDLIB_H_INCLUDED
# include <stdlib.h>
#endif /*CSL_STDLIB_H_INCLUDED*/

#ifndef CSL_STRING_H_INCLUDED
# define CSL_STRING_H_INCLUDED
# include <string.h>
#endif /*CSL_STRING_H_INCLUDED*/

#ifndef CSL_ERRNO_H_INCLUDED
# define CSL_ERRNO_H_INCLUDED
# include <errno.h>
#endif /*CSL_ERRNO_H_INCLUDED*/

#ifndef CSL_WCHAR_H_INCLUDED
#define CSL_WCHAR_H_INCLUDED
#include <wchar.h>
#endif /*CSL_WCHAR_H_INCLUDED*/

#ifndef CSL_STDARG_H_INCLUDED
#define CSL_STDARG_H_INCLUDED
#include <stdarg.h>
#endif /*CSL_STDARG_H_INCLUDED*/

/* types */
#ifndef CSL_TYPE_UNKNOWN
#define CSL_TYPE_UNKNOWN 0
#endif /*CSL_TYPE_UNKNOWN*/

#ifndef CSL_TYPE_NULL
#define CSL_TYPE_NULL 0
#endif /*CSL_TYPE_NULL*/

#ifndef CSL_TYPE_INT64
#define CSL_TYPE_INT64 1
#endif /*CSL_TYPE_INT64*/

#ifndef CSL_TYPE_DOUBLE
#define CSL_TYPE_DOUBLE 2
#endif /*CSL_TYPE_DOUBLE*/

#ifndef CSL_TYPE_STR
#define CSL_TYPE_STR 3
#endif /*CSL_TYPE_STR*/

#ifndef CSL_TYPE_USTR
#define CSL_TYPE_USTR 4
#endif /*CSL_TYPE_USTR*/

#ifndef CSL_TYPE_BIN
#define CSL_TYPE_BIN 5
#endif /*CSL_TYPE_BIN*/

/* class helpers */
#ifndef CSL_CLASS
#define CSL_CLASS(NAME) static inline const char * class_name() { return (#NAME); }
#endif /*CSL_CLASS*/

#ifndef AUTOEXEC
#define AUTOEXEC( NAMESPACE1, NAMESPACE2, TASK, FUNCTION ) \
  namespace NAMESPACE1 { \
     namespace NAMESPACE2 { \
       namespace { \
         static int TASK = FUNCTION(); \
       } /* end of anonymous namespace */ \
     } /* end of NAMESPACE2 */ \
   } /* end od NAMESPACE1 */
#endif /*AUTOEXEC*/

#endif /* _csl_common_common_h_included_ */
