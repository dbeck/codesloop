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

#ifndef _csl_db_slt3_query_hh_included_
#define _csl_db_slt3_query_hh_included_

#include "codesloop/common/pvlist.hh"
#include "codesloop/common/int64.hh"
#include "codesloop/common/dbl.hh"
#include "codesloop/common/binry.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/common/common.h"
#ifdef __cplusplus
#include <memory>

namespace csl
{
  namespace db
  {
    namespace slt3
    {
      class tran;
      class param;

      class query
      {
        public:
          query(tran & t);
          ~query();

          struct colhead
          {
            enum {
              t_null    = CSL_TYPE_NULL,
              t_integer = CSL_TYPE_INT64,
              t_double  = CSL_TYPE_DOUBLE,
              t_string  = CSL_TYPE_USTR,
              t_blob    = CSL_TYPE_BIN
            };
            
            int    type_;
            char * name_;
            char * table_;
            char * db_;
            char * origin_;
            
            colhead() : type_(t_null), name_(0), table_(0), db_(0), origin_(0) {}

            void debug();
          };

          typedef common::pvlist< 32,colhead,common::nop_destructor<colhead> > columns_t;
          typedef common::var field;
          typedef common::pvlist< 32,field,common::nop_destructor<field> > fields_t;

          common::int64 & int64_param(unsigned int pos);
          common::dbl   & dbl_param(unsigned int pos);
          common::ustr  & ustr_param(unsigned int pos);
          common::binry & binry_param(unsigned int pos);

          common::var & set_param(unsigned int pos,const common::var & p);

          void clear_params();
          bool prepare(const char * sql);
          bool reset();
          void reset_data();
          void autoreset_data(bool yesno);
          bool autoreset_data();
          bool next(columns_t & cols, fields_t & fields);
          bool next();
          long long last_insert_id();
          long long change_count();
          bool execute(const char * sql);
          bool execute(const char * sql, common::ustr & result);

	  // TODO : use EXC and OBJ macros instead
          void debug();
          void use_exc(bool yesno);
          bool use_exc();

        private:
          /* private implementation hidden in impl */
          struct impl;
          typedef std::auto_ptr<impl> impl_t;
          impl_t impl_;

          /* copying not allowed */
          query(const query & other);
          query & operator=(const query & other);

          /* no default construction */
          query();
      };
    } /* end of ns: csl::db::slt3 */
  } /* end of ns: csl::db */
} /* end of ns: csl */

#endif /* __cplusplus */
#endif /* _csl_db_slt3_query_hh_included_ */
