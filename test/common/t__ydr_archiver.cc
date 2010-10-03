/*
Copyright (c) 2008,2009,2010, CodeSLoop Team

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

#if 0
#ifdef DEBUG_BUILD
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
#endif /* DEBUG */
#endif /* DEBUG_BUILD */
#endif

#include "codesloop/common/ydr_archiver.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/test_timer.h"
#include <assert.h>

using namespace csl::common;

namespace test_ydr_archiver
{
  /*
  ** DEBUG support --------------------------------------------------------------------
  */
  static inline const wchar_t * get_namespace()   { return L"test_ydr_archiver"; }
  static inline const wchar_t * get_class_name()  { return L"test_ydr_archiver::noclass"; }
  static inline const wchar_t * get_class_short() { return L"noclass"; }
  
  void if_serialize()
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    ydr_archiver yarch( archiver::SERIALIZE, bs );
    archiver & arch(yarch);

    /* serializing a single interger on both the YDR and the generic interface */
    uint32_t x = 1;
    yarch.serialize( x );
    arch.serialize( x );
  }
  
  void if_push()
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    ydr_archiver yarch( archiver::SERIALIZE, bs );
    archiver & arch(yarch);

    /* pushing a string into the archiver */
    str s(L"Hello world");
    yarch.push( yarch.get_stream(), s );
    arch.push( arch.get_stream(), s );
  }
  
  void if_pop()
  {
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    ydr_archiver yarch( archiver::SERIALIZE, bs );
    archiver & arch(yarch);

    /* pushing strings into the archiver */
    str s(L"Hello world");
    yarch.push( yarch.get_stream(), s );
    arch.push( arch.get_stream(), s );
    
    /* pop as ustr */
    ustr res;
    uint32_t timeout = 50;
    yarch.pop( yarch.get_stream(), res, timeout );
    arch.pop( arch.get_stream(), res, timeout );
  }

} /* end of test_ydr_archiver */

using namespace test_ydr_archiver;

int main()
{
  /* interface tests */
  if_serialize();
  if_push();
  if_pop();
  
  //csl_common_print_results( "dummy         ", csl_common_test_timer_v0(dummy),"" );
  return 0;  
}

/* EOF */

