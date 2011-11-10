/*
Copyright (c) 2008,2009,2010,2011 CodeSLoop Team

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

#include "codesloop/common/lgr_msg.hh"
#include "codesloop/common/logger.hh"

namespace csl
{
  namespace common
  {
    namespace lgr
    {
      msg::msg(lgr::loc & l, logger_base & lb) :
          loc_(&l),
          logger_(&lb),
          data_(lb.construct_msgdata()),
          used_(0),
          eor_seen_(false)
      {
        append<unsigned int>(loc_->locid());
      }

      msg::~msg()
      {
        if( loc_ && loc_->level() == logger_base::scoped_ && eor_seen_ )
        {
          data_ = logger_->construct_msgdata();
          append<unsigned int>(loc_->locid());
          *this << return_from_function();
        }
      }

      void msg::flush()
      {
        CSL_REQUIRE( data_.is_valid() );
        if( data_.is_valid() )
        {
          common::ksmsg m(
              common::ksmsg::buf(data_.buf(),used_),
              data_.get_lock());
          data_.clear();
          used_ = 0;
          logger_->push(m);
        }
      }

      void msg::append(const void * data, size_t len)
      {
        CSL_REQUIRE( data != NULL );
        CSL_REQUIRE( len != 0 );
        long remaining = data_.len() - used_;
        if( remaining > static_cast<long>(len) )
        {
          ::memcpy(data_.buf()+used_,data,len);
          used_ += len;
        }
      }

      msg & msg::operator<<(const char * str)
      {
        CSL_REQUIRE( str != NULL );
        // append type
        append<unsigned char>(type_select<const char *>::sel_);
        // append length
        unsigned short sz = static_cast<unsigned short>(::strlen(str));
        append<unsigned short>(sz);
        // append data
        if( sz > 0 ) append(str,sz);
        return *this;
      }

#ifndef CSL_DECLARE_LGR_TYPE_SELECT_SEL
#define CSL_DECLARE_LGR_TYPE_SELECT_SEL(TYPE) \
      const unsigned char type_select<TYPE>::sel_ = type_select<TYPE>::val_
#endif // CSL_DECLARE_LGR_TYPE_SELECT_SEL

      CSL_DECLARE_LGR_TYPE_SELECT_SEL(bool);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(int8_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(int16_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(int32_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(int64_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(uint8_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(uint16_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(uint32_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(uint64_t);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(float);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(double);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(tag);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(name);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(end_of_record);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(return_from_function);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(const char *);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(const wchar_t *);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(when);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(threadid);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(procid);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(hostid);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(seqno);
      CSL_DECLARE_LGR_TYPE_SELECT_SEL(max_value);
    }
  }
}
