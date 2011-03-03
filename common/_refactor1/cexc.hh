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

#ifndef _csl_common_cexc_hh_included_
#define _csl_common_cexc_hh_included_

#include "codesloop/common/str.hh"
#include "codesloop/common/logger.hh"
#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    class cexc : public serializable
    {
      public:
        static const wchar_t * reason_string(int rc);

        virtual void to_string(str & res);

        virtual str to_string();

        cexc(const wchar_t * component)
          : reason_(0), component_(component), line_(0) {}

        cexc(int reason, const wchar_t * component)
          : reason_(reason), component_(component), line_(0) {}

        cexc(int reason, const wchar_t * component, const wchar_t * txt)
          : reason_(reason), component_(component), text_(txt), line_(0) {}

        cexc(int reason, const wchar_t * component, const wchar_t * txt, const wchar_t * file, unsigned int line)
          : reason_(reason), component_(component), text_(txt), file_(file), line_(line) {}


        virtual void serialize(class archiver & ar);

        virtual ~cexc();

        int reason_;
        str component_;
        str text_;
        str file_;
        unsigned int line_;

      protected:
        cexc();
    };
  }
}

#ifdef __cplusplus

#ifndef THR
#define THR(REASON,RET) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON)); \
      if( this->use_exc() ) \
            throw exc(REASON,get_class_name(),L"",L""__FILE__,__LINE__); \
      RETURN_FUNCTION( RET ); \
    } while(false);
#endif /*THR*/

#ifndef THRNORET
#define THRNORET(REASON) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON)); \
      if( this->use_exc() )  \
          throw exc(REASON,get_class_name(),L"",L""__FILE__,__LINE__); \
    } while(false);
#endif /*THRNORET*/

#ifndef THRR
#define THRR(REASON,MSG,RET) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON), \
                 MSG ); \
      if( this->use_exc() ) \
            throw exc(REASON,get_class_name(),MSG,L""__FILE__,__LINE__); \
      RETURN_FUNCTION( RET ); \
    } while(false);
#endif /*THRR*/

#ifndef THRS
#define THRS(REASON,MSG,RET) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON), \
                 MSG ); \
      if( use_exc() ) \
            throw exc(REASON,get_class_name(),MSG,L""__FILE__,__LINE__); \
      RETURN_FUNCTION( RET ); \
    } while(false);
#endif /*THRS*/

#ifndef THRRNORET
#define THRRNORET(REASON,MSG) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON), \
                 MSG ); \
      if( this->use_exc() ) \
            throw exc(REASON,get_class_name(),MSG,L""__FILE__,__LINE__); \
    } while(false);
#endif /*THRRNORET*/

#ifndef THRSNORET
#define THRSNORET(REASON,MSG) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON), \
                 MSG ); \
      if( use_exc() ) \
            throw exc(REASON,get_class_name(),MSG,L""__FILE__,__LINE__); \
    } while(false);
#endif /*THRSNORET*/

#ifndef THRC
#define THRC(REASON,RET) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls]\n", \
                 L""__FILE__,__LINE__, \
                 get_class_name(), \
                 exc::reason_string(REASON)); \
      if( this->use_exc() ) \
      { \
        wchar_t errstr[256]; \
        mbstowcs( errstr,strerror(errno),255 ); \
        throw exc(REASON,get_class_name(),errstr,L""__FILE__,__LINE__); \
      } \
      RETURN_FUNCTION( RET ); \
    } while(false);
#endif /*THRC*/

#ifndef THREX
#define THREX(E,RET) \
    do { \
      CSL_DEBUGF(L"Exception(%ls:%d): [%ls] [%ls]\n", \
                 E.file_.c_str(),E.line_, \
                 get_class_name(), \
                 exc::reason_string(E.reason_)); \
      if( this->use_exc() ) { throw ( E ); } \
      RETURN_FUNCTION( RET ); \
    } while(false);
#endif /*THR*/

#endif /*__cplusplus*/

#endif /* __cplusplus */
#endif /* _csl_common_cexc_hh_included_ */
