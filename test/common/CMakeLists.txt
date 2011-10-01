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

# -- test / common --

LINK_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}../../common )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../../.. )
LINK_LIBRARIES( csl_common ${PTHREAD_LIBRARY} )

ADD_EXECUTABLE( perf_stpodary   perf_stpodary.cc )
ADD_EXECUTABLE( perf_str        perf_str.cc )
ADD_EXECUTABLE( perf_bitmap512  perf_bitmap512.cc )
ADD_EXECUTABLE( perf_bithacks   perf_bithacks.cc )
ADD_EXECUTABLE( perf_bufmgr     perf_bufmgr.cc )
ADD_EXECUTABLE( feas_allocator  feas_allocator.cc )
ADD_EXECUTABLE( feas_bits       feas_bits.cc )
ADD_EXECUTABLE( feas_logger     feas_logger.cc )
ADD_EXECUTABLE( feas_lock       feas_lock.cc )
ADD_EXECUTABLE( feas_atomic     feas_atomic.cc )
ADD_EXECUTABLE( feas_archiver   feas_archiver.cc )
ADD_EXECUTABLE( feas_0x         feas_0x.cc )
ADD_EXECUTABLE( feas_kspin      feas_kspin.cc )
ADD_EXECUTABLE( feas_ksbuf      feas_ksbuf.cc )
ADD_EXECUTABLE( func_stpodary   func_stpodary.cc )
ADD_EXECUTABLE( func_str        func_str.cc )
ADD_EXECUTABLE( func_exc        func_exc.cc )
ADD_EXECUTABLE( func_bitmap512  func_bitmap512.cc )
ADD_EXECUTABLE( func_bithacks   func_bithacks.cc )
ADD_TEST( func_common_stpodary    ${EXECUTABLE_OUTPUT_PATH}/func_stpodary )
ADD_TEST( func_common_str         ${EXECUTABLE_OUTPUT_PATH}/func_str )
ADD_TEST( func_common_exc         ${EXECUTABLE_OUTPUT_PATH}/func_exc )
ADD_TEST( func_common_bitmap512   ${EXECUTABLE_OUTPUT_PATH}/func_bitmap512 )
ADD_TEST( func_common_bithacks    ${EXECUTABLE_OUTPUT_PATH}/func_bithacks )

# -- EOF --
