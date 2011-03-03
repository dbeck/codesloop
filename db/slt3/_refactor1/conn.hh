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

#ifndef _csl_db_slt3_conn_hh_included_
#define _csl_db_slt3_conn_hh_included_

#include "codesloop/common/ustr.hh"
#ifdef __cplusplus
#include <memory>

namespace csl
{
  namespace db
  {
    namespace slt3
    {
      class tran;

      class conn
      {
        public:

          conn();
          ~conn();

          bool open(const char * db);
          const common::ustr & name() const;
          bool close();
          long long last_insert_id();
          long long change_count();
	  // TODO : use EXC macro instead
          void use_exc(bool yesno);
          bool use_exc();

          /* types */
          struct impl;
          typedef std::auto_ptr<impl> impl_t;

        private:
          /* private data */
          friend class tran;
          impl_t impl_;
          /* copying not allowed */
          conn(const conn & other);
          conn & operator=(const conn & other);
      };
    } /* end of ns: csl::db::slt3 */
  } /* end of ns: csl::db */
} /* end of ns: csl */


#endif /* __cplusplus */
#endif /* _csl_db_slt3_conn_hh_included_ */
