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

#include "codesloop/common/arch.hh"
#include "codesloop/common/exc.hh"
#include "codesloop/common/var.hh"
#include "codesloop/common/xdrbuf.hh"

namespace csl
{
  namespace common
  {
    arch_old0::arch_old0( direction d )
      : direction_(d)
    {
      pbuf_ = new pbuf;
      xdrbuf_ = new xdrbuf(*pbuf_);

      if ( !pbuf_ || !xdrbuf_ )
        THRNORET(exc::rs_out_of_memory);
    }

    arch_old0::~arch_old0()
    {
      if ( pbuf_ )
        delete pbuf_;
      if ( xdrbuf_ )
        delete xdrbuf_;
    }

    uint64_t arch_old0::size() const
    {
      return pbuf_->size();
    }

    pbuf * arch_old0::get_pbuf() const
    {
      return pbuf_;
    }

    void arch_old0::set_pbuf( const pbuf & src )
    {
      if (  &src != pbuf_) 
      {
        *pbuf_ = src;
        if ( xdrbuf_ )
          delete xdrbuf_;
      }

      xdrbuf_ = new xdrbuf(*pbuf_);

      if ( !xdrbuf_ )
        THRNORET(exc::rs_out_of_memory);
    }
    
    void arch_old0::set_direction( direction d) { 
      reset();
      direction_ = d;
    }

    void arch_old0::reset() {
      xdrbuf_->rewind();
      pbuf_->free_all();
    }
  }
}

/* EOF */
