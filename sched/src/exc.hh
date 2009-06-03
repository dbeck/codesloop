/*
Copyright (c) 2008,2009, David Beck

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

#ifndef _csl_sched_exc_hh_included_
#define _csl_sched_exc_hh_included_

/**
   @file csl_sched/src/exc.hh
   @brief common exception class for sched
 */

#include "str.hh"
#ifdef __cplusplus

namespace csl
{
  namespace sched
  {
    /**
    @brief common exception class used by sched classes

    this class is used by the sched classes as an exception to be thrown
    */
    class exc
    {
      public:
        enum {
          rs_unknown,        ///<Unknown error.
          rs_invalid_param,  ///<Invalid parameter received
          rs_database_error, ///<Database error
          rs_insufficient,   ///<More data needed
          rs_xdr,            ///<XDR conversion error
          rs_private_fun,    ///<Function is private
        };

        enum {
          cm_unknown,     ///<Unknown component
          cm_cfg,         ///<cfg component
          cm_data,        ///<data component
          cm_head,        ///<peer component
          cm_item,        ///<item component
          cm_peer,        ///<peer component
          cm_schedule,    ///<schedule component
          cm_item_state,  ///<item state component
          cm_remote_peer, ///<remote peer component
          cm_route,       ///<route component
        };

        /** @brief converts reason code to string */
        static const char * reason_string(int rc);

        /** @brief converts component code to string */
        static const char * component_string(int cm);

        /** @brief converts exception to string */
        void to_string(common::str & res);

        /** @brief constructor 
        @param component that caused the exception */
        exc(int component)
        : reason_(rs_unknown), component_(component), line_(0) {}

        /** @brief constructor
        @param reason is to tell why
        @param component that cause the exception */
        exc(int reason, int component)
        : reason_(reason), component_(component), line_(0) {}

        /** @brief constructor
        @param reason is to tell why
        @param component that cause the exception
        @param txt provides some explanation */
        exc(int reason, int component, const wchar_t * txt)
        : reason_(reason), component_(component), text_(txt), line_(0) {}

        /** @brief constructor
        @param reason is to tell why
        @param component that cause the exception
        @param txt provides some explanation
        @param file tells which source file caused the error
        @param lin tells which line cause the error */
        exc(int reason, int component, const wchar_t * txt, const wchar_t * file, unsigned int line)
        : reason_(reason), component_(component), text_(txt), file_(file), line_(line) {}

        ~exc();

        int reason_;        ///<reason code: one of rs_*
        int component_;     ///<component code: one of cm_*
	common::str text_;  ///<error explanation
	common::str file_;  ///<error source file
        unsigned int line_; ///<error posintion in source file

      private:
        exc();
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_sched_exc_hh_included_ */
