# Copyright (c) 2008,2009,2010, CodeSLoop Team
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# SET(CMAKE_VERBOSE_MAKEFILE ON)

# -- test / sched --

LINK_LIBRARIES( csl_sched 
                csl_db_slt3
                csl_db
                csl_sec
                csl_nthread
                csl_common
                ${OPENSSL_LIBS}
                ${ZLIB_LIBRARY}
                ${WSOCK32_LIBRARY}
                ${PTHREAD_LIBRARY}
                ${DLOPEN_LIBRARY} )

INCLUDE_DIRECTORIES( ../../.. ${OPENSSL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} )

LINK_DIRECTORIES( ../../sched
                  ../../common 
                  ../../nthread 
                  ../../sec
                  ../../db
                  ${OPENSSL_LIBRARY_DIR}
                  ${ZLIB_LIBRARY_DIR} )

ADD_EXECUTABLE( t__peer      t__peer.cc )
ADD_EXECUTABLE( t__schedule  t__schedule.cc )
ADD_EXECUTABLE( t__item      t__item.cc )
#ADD_EXECUTABLE( t__states   t__states.cc )
#ADD_EXECUTABLE( t__send_msg t__send_msg.cc )
#ADD_EXECUTABLE( t__recv_msg t__recv_msg.cc )
ADD_EXECUTABLE( t__iface     t__iface.cc )

# -- EOF --
