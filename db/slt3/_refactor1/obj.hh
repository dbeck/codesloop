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

#ifndef _csl_db_obj_hh_included_
#define _csl_db_obj_hh_included_

#include "codesloop/db/slt3/var.hh"
#include "codesloop/db/slt3/conn.hh"
#include "codesloop/db/slt3/tran.hh"
#include "codesloop/db/slt3/sql.hh"
#include "codesloop/common/csl_common.hh"
#ifdef __cplusplus

namespace csl
{
  namespace db
  {
    namespace slt3
    {
      class obj : public csl::common::obj
      {
        public:

          inline obj() : changed_(false) {}
          virtual ~obj() {}

          virtual conn & db() = 0;
          virtual sql::helper & sql_helper() const = 0;
          virtual void set_id(long long id);
          virtual bool init(tran & t);
          virtual bool create(tran & t);
          virtual bool save(tran & t);
          virtual bool remove(tran & t);
          virtual bool find_by_id(tran & t);
          virtual bool find_by(tran & t,
                               int field1,
                               int field2=-1,
                               int field3=-1,
                               int field4=-1,
                               int field5=-1);
          virtual bool init();
          virtual bool create();
          virtual bool save();
          virtual bool remove();
          virtual bool find_by_id();
          virtual bool find_by(int field1,
                               int field2=-1,
                               int field3=-1,
                               int field4=-1,
                               int field5=-1);
          virtual void on_load() {}
          virtual void on_change() { changed_ = true; }
          virtual const char * table_name() const { return sql_helper().table_name(); }
          virtual var_base::helper & var_helper() { return var_helper_; }

          inline void changed(bool c) { changed_ = c; }
          inline bool changed() { return changed_; }

        private:
          var_base::helper var_helper_;
          bool changed_;
          inline obj(const obj & other) {}
          inline obj & operator=(const obj & other) { return *this; }

          CSL_OBJ(csl::slt3,obj);
      };
    }; /* end of csl::db::slt3 namespace */
  }; /* end of csl::db namespace */
}; /* end of csl namespace */

#endif /* __cplusplus */
#endif /* _csl_db_obj_hh_included_ */
