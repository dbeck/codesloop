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

#ifndef _csl_comm_bfd_hh_included_
#define _csl_comm_bfd_hh_included_

#include "codesloop/comm/sai.hh"
#include "codesloop/common/rdbuf.hh"
#include "codesloop/common/read_res.hh"
#include "codesloop/common/common.h"
#include "codesloop/common/obj.hh"
#ifdef __cplusplus

namespace csl
{
  using common::rdbuf;
  using common::read_res;

  namespace comm
  {
    class bfd
    {
      public:
        bfd();
        bfd(int fd);

        ~bfd();

        void init(int fd);

        read_res & read(uint64_t sz, uint32_t & timeout_ms, read_res & rr);
        read_res & recv(uint64_t sz, uint32_t & timeout_ms, read_res & rr);
        read_res & recvfrom(uint64_t sz, SAI & from, uint32_t & timeout_ms, read_res & rr);

        uint64_t read_some(uint32_t & timeout_ms);
        uint64_t recv_some(uint32_t & timeout_ms);
        uint64_t recvfrom_some(SAI & from, uint32_t & timeout_ms);

        bool write(const uint8_t * data, uint64_t sz);
        bool send(const uint8_t * data, uint64_t sz);
        bool sendto(const uint8_t * data, uint64_t sz,const SAI & to);

        static const int ok_                =  0;
        static const int unknonwn_error_    = -1;
        static const int not_initialized_   = -2;
        static const int closed_            = -3;
        static const int fd_error_          = -4;
        static const uint64_t max_size_     = 256*1024;

        int state() const;
        uint64_t size() const; 
        uint64_t n_free() const;

        bool can_read(uint32_t & timeout_ms);
        bool read_buf(read_res & res, uint64_t sz);
        void shutdown();
        void close();

        int file_descriptor() const { return fd_; }

        const unsigned char * data() const { return buf_.data(); }

      private:
        typedef rdbuf<512,max_size_> buf_t;

        static const int read_op_      = 1;
        static const int recv_op_      = 2;
        static const int recvfrom_op_  = 3;

        uint64_t internal_read( int op_type,
                                SAI & from,
                                uint32_t & timeout_ms );
        int        fd_;
        buf_t      buf_;

        CSL_OBJ(csl::comm,bfd);
        USE_EXC();
    };
  }
}

#endif /*__cplusplus*/
#endif /* _csl_comm_bfd_hh_included_ */
