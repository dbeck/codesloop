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
 
#ifndef _csl_rpc_cli_trans_hh_included_
#define _csl_rpc_cli_trans_hh_included_

#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#include "codesloop/common/pbuf.hh"
#include "codesloop/common/inpvec.hh"
#include "codesloop/rpc/handle.hh"
#include "codesloop/comm/tcp_client_old0.hh"
#ifdef __cplusplus

#include <map>

namespace csl 
{ 
  namespace rpc 
  {
    class cli_trans : public csl::common::obj
    {
      CSL_OBJ(csl::rpc,cli_trans);

    protected:
      enum global_function_ids {   
        fid_ping = 1,
      };  

      enum return_type {
        rt_succcess = 0,
        rt_exception = 1
      };


      virtual void ping(uint64_t&, uint64_t*) = 0;
      virtual void ping(csl::rpc::handle &,
                        uint64_t &  client_time,
                        uint64_t *  server_time) = 0;
      
      virtual void decode_response(
          const csl::rpc::handle &,
          const uint32_t,
          csl::common::archiver &) = 0;

    protected:
      void create_handle(handle &);
      virtual void wait(handle &) = 0;
      virtual void send(handle &, csl::common::pbuf *) = 0;

      typedef csl::common::inpvec<void *> ptr_ivec_t;
      typedef std::pair<uint32_t, ptr_ivec_t *> out_params_t;
      typedef std::pair<handle,out_params_t> handle_params_pair_t;
      typedef std::map<handle,out_params_t> output_ptr_vec_t;
    
      output_ptr_vec_t outp_ptrs_;
    };


  }
}

#endif /* __cplusplus */
#endif /* _csl_rpc_cli_trans_hh_included_ */
