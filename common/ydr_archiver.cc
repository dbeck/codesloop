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

#include "codesloop/common/ydr_archiver.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/var.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/common/dbl.hh"
#include "codesloop/common/int64.hh"
#include "codesloop/common/binry.hh"

namespace csl
{
  namespace common
  {  
    ydr_archiver::ydr_archiver( direction d, stream_base & s, uint32_t timeout_ms ) 
      : archiver(d,s,timeout_ms) {}

    void ydr_archiver::reset() {}

    archiver & ydr_archiver::serialize( str & v ) { return *this; }
    archiver & ydr_archiver::serialize( ustr & v ) { return *this; }
    archiver & ydr_archiver::serialize( dbl & v ) { return *this; }
    archiver & ydr_archiver::serialize( int64 & v ) { return *this; }
    archiver & ydr_archiver::serialize( binry & v ) { return *this; }

    archiver & ydr_archiver::serialize( int8_t & v ) { return *this; }
    archiver & ydr_archiver::serialize( int16_t & v ) { return *this; }
    archiver & ydr_archiver::serialize( int32_t & v ) { return *this; }
    archiver & ydr_archiver::serialize( int64_t & v ) { return *this; }
        
    archiver & ydr_archiver::serialize( uint8_t & v ) { return *this; }
    archiver & ydr_archiver::serialize( uint16_t & v ) { return *this; }
    archiver & ydr_archiver::serialize( uint32_t & v ) { return *this; }
    archiver & ydr_archiver::serialize( uint64_t & v ) { return *this; }

    archiver & ydr_archiver::serialize( float & v ) { return *this; }
    archiver & ydr_archiver::serialize( double & v ) { return *this; }
  }
}

/* EOF */

