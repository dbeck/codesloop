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
 
#ifndef _csl_rpc_iface_hh_included_
#define _csl_rpc_iface_hh_included_

#include "codesloop/common/common.h"
#ifdef __cplusplus
#include <string>
#include <vector>
#include "codesloop/common/obj.hh"
#include "codesloop/rpc/csrgen.hh"

namespace csl 
{ 
  namespace rpc 
  {
    class iface : public csl::common::obj
    {
      CSL_OBJ(csl::rpc,iface);

      public:
        struct param {
          std::string type;
          std::string name;
          param_kind kind;
          bool is_array;
          size_t array_length;
        };
        
        struct func {
          std::string name;
          bool disposable;
          std::vector<param> params;
          std::vector<func> callbacks;
          typedef std::vector<param>::const_iterator param_iterator;
          typedef std::vector<func>::const_iterator callback_iterator;
        };

        /*
         * setters
         */
        void set_name(const token_info &);
        void set_version(const token_info &);
        void set_namespc(const token_info &);
        void set_transport(const token_info &);

        void add_function(const token_info &);
        void add_callback(const token_info &);
        void set_param_type(const token_info &);
        void set_param_name(const token_info &);
        void set_arry_len(int);

        void add_include(const token_info &);   

        /*
         * getters 
         */
        const std::string get_name() const { return name_;}
        const std::string get_version() const { return version_;}
        const std::string get_namespc() const { return namespc_;}
        const std::string get_transport() const 
        { 
          return transport_ == "" ? "udp" : transport_;
        }
        const std::vector<func> * get_functions() const 
        {
          return &functions_;
        }

        const std::vector<std::string> * get_includes() const
        {
          return &includes_;
        }

        std::string to_string() const;        

        typedef std::vector<std::string>::const_iterator include_iterator;
        typedef std::vector<func>::const_iterator function_iterator;
      private:
        std::string name_;
        std::string version_;
        std::string namespc_;
        std::string transport_;

        std::string token_to_string(const token_info & ) const;
        std::string param_type_;

        std::vector<func> functions_;
        std::vector<std::string> includes_;
    };


  }
}

#endif /* __cplusplus */
#endif /* _csl_rpc_iface_hh_included_ */
