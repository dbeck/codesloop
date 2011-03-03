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


#ifndef DEBUG
#define DEBUG
#endif /* DEBUG */

#include "codesloop/common/common.h"
#include "codesloop/common/str.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/ydr_archiver.hh"
#include "codesloop/common/logger.hh"
#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream.hh"
#include <assert.h>
#include <sys/stat.h>

using namespace csl::common;

int main()
{
  bool caught = false;

  try {
    str src = L"This is a test";
    str dst;
    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);

    // Class types - direct
    ydr_archiver sar( archiver::SERIALIZE, bs );
    src.serialize( sar );

    ydr_archiver dar( archiver::DESERIALIZE, bs );
    dst.serialize( dar);

    assert( src == dst );

    // Elementary types
    unsigned int elementary = 0xDeadBabe;
    ydr_archiver sar2( archiver::SERIALIZE, bs );
    sar2.serialize( elementary );

    elementary = 0;

    ydr_archiver dar2( archiver::DESERIALIZE, bs );
    dar2.serialize( elementary );

    assert( elementary == 0xDeadBabe );


  } catch ( exc e ) {
    FPRINTF(stderr,L"Exception caught: %ls\n",e.to_string().c_str());
    caught = true;
  }
  /* this should not throw an exception, will add as zero length string */
  assert( caught == false );
}

/* EOF */
