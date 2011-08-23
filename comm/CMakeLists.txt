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

# -- comm --

LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}../common )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}../.. )
LINK_LIBRARIES( csl_common )

ADD_LIBRARY(
    csl_comm STATIC
    tcp_listener.cc tcp_listener.hh
    )

ADD_DEPENDENCIES( csl_comm ${CMAKE_CURRENT_SOURCE_DIR}../common/libev/config.h )

FILE(GLOB includes "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
INSTALL( FILES ${includes} DESTINATION include/codesloop/comm )
INSTALL(TARGETS csl_comm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# -- EOF --
