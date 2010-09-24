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

#ifndef _csl_common_var_hh_included_
#define _csl_common_var_hh_included_

#include "codesloop/common/obj.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/serializable.hh"
#ifdef __cplusplus
#include <string>

namespace csl
{
  namespace common
  {
    class str;
    class ustr;
    class int64;
    class dbl;
    class binry;
    class xdrbuf;

    class var : public obj, public serializable
    {
      public:
        inline var() : obj() { }
        virtual inline ~var() { }

        enum { var_type_v =  CSL_TYPE_UNKNOWN };

        inline virtual int var_type() const { return var_type_v; }

        virtual void reset() = 0;
        
        virtual bool to_integer(int64 & v) const = 0;
        virtual bool to_integer(int64_t & v) const = 0;
        virtual bool from_integer(const int64 & v) = 0;
        virtual bool from_integer(int64_t v) = 0;

        virtual bool to_double(dbl & v) const = 0;
        virtual bool to_double(double & v) const = 0;
        virtual bool from_double(const dbl & v) = 0;
        virtual bool from_double(double v) = 0;

        virtual bool to_string(str & v) const = 0;
        virtual bool to_string(ustr & v) const = 0;
        virtual bool to_string(std::string & v) const = 0;
        virtual bool from_string(const str & v) = 0;
        virtual bool from_string(const ustr & v) = 0;
        virtual bool from_string(const std::string & v) = 0;
        virtual bool from_string(const char * v) = 0;
        virtual bool from_string(const wchar_t * v) = 0;
        
        virtual bool to_binary(binry & v) const = 0;
        virtual bool to_binary(unsigned char * v, size_t & sz) const = 0;
        virtual bool to_binary(void * v, size_t & sz) const = 0;
        virtual bool from_binary(const binry & v) = 0;
        virtual bool from_binary(const unsigned char * v,size_t sz) = 0;
        virtual bool from_binary(const void * v,size_t sz) = 0;

        virtual bool to_var(var & v) const = 0;
        virtual bool from_var(const var & v) = 0;

        virtual bool to_xdr(xdrbuf & b) const = 0;
        virtual bool from_xdr(xdrbuf & v) = 0;

        virtual size_t var_size() const = 0;
        virtual const unsigned char * ucharp_data() const = 0;

        virtual inline operator int64_t () const
        {
         int64_t ret = 0;
         this->to_integer(ret);
         return ret;
        }

        virtual inline operator double () const
        {
         double ret = 0.0;
         this->to_double(ret);
         return ret;
        }

        virtual inline void operator=(int64_t v)
        {
         this->from_integer(v);
        }

        virtual inline void operator=(double v)
        {
         this->from_double(v);
        }

        virtual const char * charp_data() const { return reinterpret_cast<const char *>(this->ucharp_data()); };

        virtual inline bool set(const char * v)    { return (this->from_string(v));  }
        virtual inline bool set(const wchar_t * v) { return (this->from_string(v));  }
        virtual inline bool set(int64_t v)         { return (this->from_integer(v)); }
        virtual inline bool set(double v)          { return (this->from_double(v));  }
        virtual inline bool set(const unsigned char * ptr, 
                                size_t sz)         { return (this->from_binary(ptr,sz)); }

        virtual inline double get_double() const { return static_cast<double>(*this); }
        virtual inline int64_t get_long() const  { return static_cast<int64_t>(*this); }
        
        virtual inline bool get(int64_t & v)  { return (this->to_integer(v)); }
        virtual inline bool get(double & v)   { return (this->to_double(v)); }
        virtual inline bool get(ustr & v)     { return (this->to_string(v)); }
        virtual inline bool get(str & v)      { return (this->to_string(v)); }
        virtual inline bool get(binry & v)    { return (this->to_binary(v)); }
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_var_hh_included_ */
