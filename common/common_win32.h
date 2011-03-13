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

#ifndef _csl_common_common_win32_h_included_
#define _csl_common_common_win32_h_included_

#ifdef WIN32
# ifdef __MINGW32__
#  ifndef CSL_UNISTD_H_INCLUDED
#   define CSL_UNISTD_H_INCLUDED
#   include <unistd.h>
#  endif /*CSL_UNISTD_H_INCLUDED*/
#  ifndef CSL_SYS_TIME_H_INCLUDED
#   define CSL_SYS_TIME_H_INCLUDED
#   include <sys/time.h>
#  endif /*CSL_SYS_TIME_H_INCLUDED*/
#  ifndef CSL_TIME_H_INCLUDED
#   define CSL_TIME_H_INCLUDED
#   include <time.h>
#  endif /*CSL_TIME_H_INCLUDED*/
# endif /*__MINGW32__*/
# ifndef CSL_WINDOWS_H_INCLUDED
#  define CSL_WINDOWS_H_INCLUDED
#  include <windows.h>
# endif /*CSL_WINDOWS_H_INCLUDED*/
# ifndef ShutdownSocket
#  define ShutdownSocket(S) { ::shutdown(S,2); }
# endif /*ShutdownSocket*/
# ifndef ShutdownCloseSocket
#  define ShutdownCloseSocket(S) { ::shutdown(S,2); ::closesocket(S); }
# endif /*ShutdownCloseSocket*/
# ifndef CloseSocket
#  define CloseSocket(S) { ::closesocket(S); }
# endif /*CloseSocket*/
# ifndef Close
#  define Close(S) { ::close(S); }
# endif /*Clos*/
# ifndef SleepSeconds
#  define SleepSeconds(A) ::Sleep(A*1000)
# endif /*SleepSeconds*/
# ifndef SleepMiliseconds
#  define SleepMiliseconds(A) ::Sleep(A)
# endif /*SleepMiliseconds*/
# ifndef CSL_BASETSD_H_INCLUDED
#  define CSL_BASETSD_H_INCLUDED
#  include <BaseTsd.h>
# endif /*CSL_BASETSD_H_INCLUDED*/
# ifndef CSL_INT32_T_DEFINED
#  define CSL_INT32_T_DEFINED
   typedef INT32 int32_t;
# endif /*CSL_INT32_T_DEFINED*/
# ifndef CSL_UINT32_T_DEFINED
#  define CSL_UINT32_T_DEFINED
   typedef DWORD32 uint32_t;
# endif /*CSL_UINT32_T_DEFINED*/
# ifndef CSL_SOCKLEN_T_DEFINED
# define CSL_SOCKLEN_T_DEFINED
   typedef int socklen_t;
# endif /*CSL_SOCKLEN_T_DEFINED*/
# ifndef SNPRINTF
#  define SNPRINTF _snprintf
# endif /*SNPRINTF*/
# ifndef SWPRINTF
#  ifdef __MINGW32__
/* brain dead mingw does not support buffer size ... */
#   define SWPRINTF(BUF,SZ,FMT,...) swprintf(BUF,FMT,__VA_ARGS__)
#  else /*!__MINGW32__ && SWPRINTF && WIN32 */
#   define SWPRINTF swprintf
#  endif /*__MINGW32__ && SWPRINTF && WIN32 */
# endif /*SWPRINTF*/
# ifndef CSL_DEBUGF
#  define CSL_DEBUGF fwprintf
# endif /*CSL_DEBUGF*/
# ifndef PRINTF
#  define PRINTF wprintf
# endif /*PRINTF*/
# ifndef ATOLL
#  define ATOLL _atoi64
# endif /*ATOLL*/
# ifndef WCSTOLL
#  define WCSTOLL wcstoll
# endif /*WCSTOLL*/
# ifndef WCSTOLD
#  define WCSTOLD wcstold
# endif /*WCSTOLD*/
# ifndef WCSTOD
#  define WCSTOD wcstod
# endif /*WCSTOD*/
# ifndef UNLINK
#  define UNLINK _unlink
# endif /*UNLINK*/
# ifndef STRDUP
#  define STRDUP _strdup
# endif /*STRDUP*/
# ifndef __MINGW32__
#  define getpid()  0
# endif
# ifndef __MINGW32__
void gettimeofday(struct timeval * tv, void * p)
{
        unsigned long tc = GetTickCount();
        tv->tv_sec  = tc/1000;
        tv->tv_usec = (tc%1000)*1000;
}
#endif /* __MINGW32__ */
#endif

#endif /* _csl_common_common_win32_h_included_ */

