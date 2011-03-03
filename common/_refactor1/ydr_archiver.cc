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
#include "codesloop/common/ydr_util.hh"
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
        
    /* abstract interface */
    void ydr_archiver::reset() {}
                            
    /* separate interface for push */
    archiver & ydr_archiver::push( stream_base & b, const str & v )   { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const ustr & v )  { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const dbl & v )   { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const int64 & v ) { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const binry & v ) { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const pbuf & v )  { /* TODO ydr_push(b,v); */ return *this; }

    archiver & ydr_archiver::push( stream_base & b, const int8_t & v )  { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const int16_t & v ) { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const int32_t & v ) { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const int64_t & v ) { ydr_push(b,v); return *this; }
    
    archiver & ydr_archiver::push( stream_base & b, const uint8_t & v )  { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const uint16_t & v ) { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const uint32_t & v ) { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const uint64_t & v ) { ydr_push(b,v); return *this; }

    archiver & ydr_archiver::push( stream_base & b, const float & v )  { ydr_push(b,v); return *this; }
    archiver & ydr_archiver::push( stream_base & b, const double & v ) { ydr_push(b,v); return *this; }
    
    /* separate interface for pop */
    archiver & ydr_archiver::pop( stream_base & b, str & v, uint32_t & t )   { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, ustr & v, uint32_t & t )  { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, dbl & v, uint32_t & t )   { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, int64 & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, binry & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, pbuf & v, uint32_t & t )  { /* TODO ydr_pop(b,v,t); */ return *this; }

    archiver & ydr_archiver::pop( stream_base & b, int8_t & v, uint32_t & t )  { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, int16_t & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, int32_t & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, int64_t & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    
    archiver & ydr_archiver::pop( stream_base & b, uint8_t & v, uint32_t & t )  { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, uint16_t & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, uint32_t & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, uint64_t & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }

    archiver & ydr_archiver::pop( stream_base & b, float & v, uint32_t & t )  { ydr_pop(b,v,t); return *this; }
    archiver & ydr_archiver::pop( stream_base & b, double & v, uint32_t & t ) { ydr_pop(b,v,t); return *this; }  
  }
}

/* EOF */

