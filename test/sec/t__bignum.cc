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

#include "codesloop/sec/bignum.hh"
#include "codesloop/sec/ecdh_key.hh"
#include "codesloop/common/zfile.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include "codesloop/common/stream.hh"
#include "codesloop/common/ydr_archiver.hh"
#include <assert.h>

using namespace csl::sec;
using namespace csl::common;

namespace test_bignum {

  void baseline() { bignum bn; }

  void alloc_100()
  {
    bignum bn;
    bn.allocate(100);
  }

  void alloc_200()
  {
    bignum bn;
    bn.allocate(200);
  }

  void copy_100()
  {
    bignum bn;
    bn.allocate(100);
    bignum bn2;
    bn2 = bn;
    assert( bn2 == bn );
  }

  void copy_200()
  {
    bignum bn;
    bn.allocate(200);
    bignum bn2;
    bn2 = bn;
    assert( bn2 == bn );
  }

  void ydr()
  {
    bignum a,b;
    ecdh_key ka,kb;

    bignum c,d;

    ka.algname("prime192v3");
    kb.algname("prime192v3");

    assert( ka.gen_keypair(a) == true );
    assert( kb.gen_keypair(b) == true );

    stream_buffer<uint8_t> buf;
    buffered_stream<> bs(buf);
    ydr_archiver ar(archiver::SERIALIZE, bs);

    a.serialize(ar);
    b.serialize(ar);
    assert( buf.buflen() > 0 );

    zfile zf;
    assert( zf.put_data(buf.data(), buf.buflen()) == true );
    assert( zf.write_file("bignum.xdrbin") == true );

    ydr_archiver dar(archiver::DESERIALIZE, bs);

    c.serialize(dar);
    d.serialize(dar);

    assert( a == c );
    assert( b == d );

    assert( c.is_empty() == false );
    assert( d.is_empty() == false );
  }

  struct rndata
  {
    size_t len_;
    const char * filename_;
  };

/* TODO FIXME
  static rndata random_files[] = {
     { 15,     "random.15" },
     { 27,     "random.27" },
     { 99,     "random.99" },
     { 119,    "random.119" },
     { 279,    "random.279" },
     { 589,    "random.589" },
     { 1123,   "random.1123" },
     { 1934,   "random.1934" },
     { 28901,  "random.28901" },
     { 31965,  "random.31965" },
     { 112678, "random.112678" },
     { 0, 0 }
  };
  */

/* TODO FIXME
  void random_ydr()
  {
    rndata * p = random_files;
    while( p->len_ )
    {
      zfile zf;
      assert( zf.read_file(p->filename_) == true );
      assert( zf.get_size() == p->len_ );
      pbuf pb;
      assert( zf.get_data(pb) == true );
      xdrbuf xb(pb);
      xb.rewind();
      bignum val;
      assert( val.from_xdr(xb) == false );
      ++p;
    };
  }
*/

} // end of test_bignum

using namespace test_bignum;

int main()
{
  ydr();

  csl_common_print_results( "baseline   ", csl_common_test_timer_v0(baseline),"" );
  csl_common_print_results( "alloc_100  ", csl_common_test_timer_v0(alloc_100),"" );
  csl_common_print_results( "alloc_200  ", csl_common_test_timer_v0(alloc_200),"" );
  csl_common_print_results( "copy_100   ", csl_common_test_timer_v0(copy_100),"" );
  csl_common_print_results( "copy_200   ", csl_common_test_timer_v0(copy_200),"" );
  //csl_common_print_results( "random_ydr ", csl_common_test_timer_v0(random_ydr),"" );

  return 0;
}

/* EOF */
