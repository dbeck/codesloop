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
 
#ifndef _csl_rpc_stub_base_hh_included_
#define _csl_rpc_stub_base_hh_included_

#include "codesloop/common/common.h"
#ifdef __cplusplus
#include <fstream>
#include <string>

#include "codesloop/common/obj.hh"
#include "codesloop/rpc/iface.hh"
#include "codesloop/rpc/csrgen.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/common/str.hh"

namespace csl 
{ 
  namespace rpc 
  {
    class stub_base : public csl::common::obj
    {
      CSL_OBJ(csl::rpc,stub_base);
      
      enum stub_kind { 
        STUB_CLIENT,
        STUB_SERVER
      };

    public:
      stub_base(const iface * i);
      ~stub_base();

      virtual void generate() = 0;

      virtual void generate_func_params(std::string function, stub_kind kind, 
          bool is_async = false);
      virtual void generate_ns_open();
      virtual void generate_ns_close();

      virtual void open_file(const char * filename);

      virtual void add_internal_functions();

    protected:
      const iface * ifc_;

      csl::common::ustr ifname_;
      std::fstream output_;
      std::string ls_;

    };


  }
}

#endif /* __cplusplus */
#endif /* _csl_rpc_stub_base_hh_included_ */