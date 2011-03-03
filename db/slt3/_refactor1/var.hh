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

#ifndef _csl_db_var_hh_included_
#define _csl_db_var_hh_included_

#include "codesloop/common/pvlist.hh"
#include "codesloop/common/pbuf.hh"
#include "codesloop/common/int64.hh"
#include "codesloop/common/dbl.hh"
#include "codesloop/common/binry.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/db/slt3/query.hh"
#ifdef __cplusplus
#include <vector>

namespace csl
{
  namespace db
  {
    namespace slt3
    {
      class obj;

      class var_base
      {
      public:
        /* abstract functions */

        virtual bool set_value(query::colhead * ch,query::field * fd) = 0;
        virtual const common::var * get_value() const = 0;
        virtual int type() = 0;
        inline virtual ~var_base() {}
        inline var_base(obj & prn) : parent_(&prn) {}

      protected:
        virtual void register_variable(var_base * v, const char * name, const char * coltype, slt3::obj & parent, const char * flags);
        inline virtual obj * parent() { return parent_; }

        obj * parent_;

      public:
        class helper
        {
          public:
            bool add_field(const char * name, var_base & v);

            struct data
            {
              const char * name_;
              var_base  * var_;
              data(const char * nm,var_base & v) : name_(nm), var_(&v) {}
            };

            typedef common::pvlist< 32,data,common::delete_destructor<data> > datalist_t;

            bool init(tran & t, const char * sql_query);
            bool create(tran & t, const char * sql_query);
            bool save(tran & t, const char * sql_query);
            bool remove(tran & t, const char * sql_query);
            bool find_by_id(tran & t, const char * sql_query);
            bool find_by(tran & t,
                        const char * sql_query,
                        int field1,
                        int field2=-1,
                        int field3=-1,
                        int field4=-1,
                        int field5=-1);
            void set_id(long long id);

          private:
            datalist_t dtalst_;
        };

      private:
        /* not allowed to call these */
        var_base() {}
        var_base(const var_base & other) {}
        var_base & operator=(const var_base & other) { return *this; }
      };

      template <typename T> struct var_col_type {};
      template <> struct var_col_type<common::int64> { static const char * coltype_s; };
      template <> struct var_col_type<common::dbl> { static const char * coltype_s; };
      template <> struct var_col_type<common::ustr> { static const char * coltype_s; };
      template <> struct var_col_type<common::binry> { static const char * coltype_s; };

      template <typename T> class varT : public var_base
      {
      public:

        inline varT(const char * name, slt3::obj & prn,const char * flags="") : var_base(prn)
        {
          register_variable(this,name,var_col_type<T>::coltype_s,prn,flags);
        }

        enum { type_v = T::var_type_v };

        inline int type() { return type_v; }

        inline bool set_value(query::colhead * ch,query::field * fd)
        {
          if( !ch || !fd ) return false;
          bool ret = value_.from_var(*fd);
          // parent()->on_change(); TODO : check this!!!
          return ret;
        }

        typedef T tval_t;
        typedef typename T::value_t value_t;

        inline value_t get() const { return value_.value(); }
        template <typename V> inline bool get(V & v) const { return value_.get(v); }

        template <typename V> inline bool set(V & v)
        {
          bool ret = value_.set(v);
          if( ret ) parent()->on_change();
          return ret;
        }

        template <typename V> inline bool operator==(V & other) const
        {
          return (value_ == other);
        }

        inline varT & operator=(const char * other)
        {
          bool success = value_.from_string(other);
          if( success ) parent()->on_change();
          return *this;
        }

        inline varT & operator=(const wchar_t * other)
        {
          bool success = value_.from_string(other);
          if( success ) parent()->on_change();
          return *this;
        }

        inline varT & operator=(const T & other)
        {
          bool success = value_.from_var(other);
          if( success ) parent()->on_change();
          return *this;
        }

        inline varT & operator=(const varT & other)
        {
          bool success = value_.from_var(other.value_);
          if( success ) parent()->on_change();
          return *this;
        }

        inline varT & operator=(const common::binry::buf_t & other)
        {
          bool success = value_.from_binary(other.data(),other.size());
          if( success ) parent()->on_change();
          return *this;
        }

        template <typename X>
        inline varT & operator=(const X & other)
        {
          T tmp(other);
          bool success = value_.from_var(tmp);
          if( success ) parent()->on_change();
          return *this;
        }

        inline varT & operator=(const std::vector<unsigned char> & vref)
        {
          bool success = value_.from_binary( &(vref[0]), vref.size() );
          if( success ) parent()->on_change();
          return *this;
        }

        inline const common::var * get_value() const { return &value_; }

      private:
        T value_;
      };

      typedef varT<common::int64> intvar;
      typedef varT<common::dbl>   doublevar;
      typedef varT<common::ustr>  strvar;
      typedef varT<common::binry> blobvar;

    } /* end of slt3 namespace */
  } /* end of db namespace */
} /* end of csl namespace */

#endif /* __cplusplus */
#endif /* _csl_db_var_hh_included_ */
