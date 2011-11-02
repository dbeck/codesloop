# Copyright (c) 2008,2009,2010,2011 CodeSLoop Team
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

# -- test / comm --

LINK_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}../../comm
    ${CMAKE_CURRENT_SOURCE_DIR}../../common
    ${DLOPEN_LIBRARY_DIR}
    )

LINK_LIBRARIES(
    csl_comm
    csl_common
    ${PTHREAD_LIBRARY}
    ${DLOPEN_LIBRARY}
    )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ${DLOPEN_INCLUDE_DIR} )

ADD_EXECUTABLE( feas_tcp_listener feas_tcp_listener.cc )
ADD_EXECUTABLE( feas_udp_listener feas_udp_listener.cc )
ADD_EXECUTABLE( perf_addr         perf_addr.cc )
ADD_EXECUTABLE( func_autofd       func_autofd.cc )

ADD_TEST( func_comm_autofd ${EXECUTABLE_OUTPUT_PATH}/func_autofd )

# -- EOF --

