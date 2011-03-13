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

#include "codesloop/sec/exc.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/str.hh"

namespace csl
{
  namespace sec
  {
    const wchar_t * exc::reason_string(int rc)
    {
      switch( rc )
      {
        case rs_salt_size:        return L"Invalid salt size.";
        case rs_null_key:         return L"Null key received.";
        case rs_too_big:          return L"Data too big.";
        case rs_rand_failed:      return L"Cannot generate random.";
        case rs_header_size:      return L"Invalid header size.";
        case rs_footer_size:      return L"Invalid footer size.";
        case rs_null_data:        return L"Data too short for encryption.";
        case rs_cksum:            return L"Checksum mismatch.";
        case rs_unknown:
          default:               return L"Unknown reason";
      };
    }
  };
};

/* EOF */