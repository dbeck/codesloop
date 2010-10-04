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

#ifndef _csl_sec_ecdh_key_hh_included_
#define _csl_sec_ecdh_key_hh_included_

#include "codesloop/sec/bignum.hh"
#include "codesloop/common/serializable.hh"
#include "codesloop/common/ustr.hh"
#include "codesloop/common/pbuf.hh"
#ifdef __cplusplus

namespace csl
{
  namespace sec
  {
    class ecdh_key : public csl::common::obj, public csl::common::serializable
    {
      public:
        ecdh_key();
        ~ecdh_key();
        ecdh_key(const ecdh_key & other);
        ecdh_key & operator=(const ecdh_key & other);

        void serialize(common::archiver & a);

        void set(const common::ustr & algname, const bignum & x, const bignum & y);

        static unsigned int algorithm_strength(const common::ustr & algname);
        
        unsigned int strength() const;
        bool gen_keypair(bignum & private_key);
        bool gen_sha1hex_shared_key(const bignum & peer_private_key, common::ustr & shared_key) const;
        bool gen_shared_key(const bignum & peer_private_key, common::pbuf & shared_key) const;

        void print() const;

        void x(const bignum & v)              { x_ = v; } 
        void y(const bignum & v)              { y_ = v; } 
        void algname(const common::ustr & v)  { algname_ = v; }
        void algname(const char * v)          { algname_ = v; }

        bignum & x()              { return x_; }
        bignum & y()              { return y_; }
        common::ustr & algname()  { return algname_; }

        const bignum & x() const              { return x_; }
        const bignum & y() const              { return y_; }
        const common::ustr & algname() const  { return algname_; }

        bool has_data() const { return (x_.is_empty()==false && y_.is_empty()==false && algname_.size()>0); }
        bool is_empty() const { return (has_data()==false); }

        bool operator==(const ecdh_key & other) const
        {
          if( !(algname_ == other.algname_) ) return false;
          if( !(x_ == other.x_) )             return false;
          if( !(y_ == other.y_) )             return false;
          return true;
        }

      private:
        common::ustr   algname_;
        bignum         x_;
        bignum         y_;
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_sec_ecdh_key_hh_included_ */
