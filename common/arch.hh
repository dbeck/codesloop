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

#ifndef _csl_common_arch_hh_included_
#define _csl_common_arch_hh_included_


#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#include "codesloop/common/xdrbuf.hh"
#ifdef __cplusplus
#include <utility>

namespace csl
{
  namespace common
  {
    class var;
    class pbuf;
    class xdrbuf;

    class arch_old0
    {
      public:
        enum direction {
          SERIALIZE,
          DESERIALIZE
        };

        arch_old0( direction d );
        virtual ~arch_old0();

        template <typename T> void serialize(T & val)
        {
          if ( direction_ == SERIALIZE )
            (*xdrbuf_) << val;
          else
            (*xdrbuf_) >> val;
        }

        uint64_t size() const;
        pbuf * get_pbuf() const;
        void set_pbuf( const pbuf & src );
        void set_direction( direction d);
        void reset();

      private:
        pbuf * pbuf_;
        xdrbuf * xdrbuf_;
        direction direction_;
        
      CSL_OBJ(csl::common,arch_old0);
      USE_EXC();
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_arch_hh_included_ */
