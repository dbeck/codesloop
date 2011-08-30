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

#ifndef _csl_common_common_unix_h_included_
#define _csl_common_common_unix_h_included_

#ifndef WIN32
# ifndef CSL_SIGNAL_H_INCLUDED
#  define CSL_SIGNAL_H_INCLUDED
#  include <signal.h>
# endif /* CSL_SIGNAL_H_INCLUDED */
# ifndef CSL_ARPA_INET_H_INCLUDED
#  define CSL_ARPA_INET_H_INCLUDED
#  include <arpa/inet.h>
# endif /*CSL_ARPA_INET_H_INCLUDED*/
# ifndef CSL_NETINET_IN_H_INCLUDED
#  define CSL_NETINET_IN_H_INCLUDED
#  include <netinet/in.h>
# endif /*CSL_NETINET_IN_H_INCLUDED*/
# ifndef CSL_UNISTD_H_INCLUDED
#  define CSL_UNISTD_H_INCLUDED
#  include <unistd.h>
# endif /*CSL_UNISTD_H_INCLUDED*/
# ifndef CSL_SYS_SOCKET_H_INCLUDED
#  define CSL_SYS_SOCKET_H_INCLUDED
#  include <sys/socket.h>
# endif /*CSL_SYS_SOCKET_H_INCLUDED*/
# ifndef CSL_SYS_TIME_H_INCLUDED
#  define CSL_SYS_TIME_H_INCLUDED
#  include <sys/time.h>
# endif /*CSL_SYS_TIME_H_INCLUDED*/
# ifndef CSL_ENDIAN_H_INCLUDED
#  ifdef LINUX
#   include <endian.h>
#  endif
#  ifdef FREEBSD
#   include <sys/endian.h>
#  endif
# endif /*CSL_ENDIAN_H_INCLUDED*/
# ifndef SleepSeconds
#  define SleepSeconds(A) ::sleep((A))
# endif /*SleepSeconds*/
# ifndef SleepMiliseconds
#  define SleepMiliseconds(A) ::usleep((A)*1000)
# endif /*SleepMiliseconds*/
# ifndef ShutdownSocket
#  define ShutdownSocket(S) { ::shutdown(S,2); }
# endif /*ShutdownSocket*/
# ifndef ShutdownCloseSocket
#  define ShutdownCloseSocket(S) { ::shutdown(S,2); ::close(S); }
# endif /*ShutdownCloseSocket*/
# ifndef CloseSocket
#  define CloseSocket(S) { ::close(S); }
# endif /*CloseSocket*/
# ifndef Close
#  define Close(S) { ::close(S); }
# endif /*Close*/
# ifndef SNPRINTF
#  define SNPRINTF snprintf
# endif /*SNPRINTF*/
# ifndef SWPRINTF
#  define SWPRINTF swprintf
# endif /*SWPRINTF*/
# ifndef FPRINTF
#  define FPRINTF fwprintf
# endif /*FPRINTF*/
# ifndef PRINTF
#  define PRINTF wprintf
# endif /*PRINTF*/
# ifndef ATOLL
#  define ATOLL atoll
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
#  define UNLINK unlink
# endif /*UNLINK*/
# ifndef STRDUP
#  define STRDUP strdup
# endif /*STRDUP*/
#endif

#endif /* _csl_common_common_unix_h_included_ */

