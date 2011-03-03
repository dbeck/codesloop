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

#ifndef _csl_common_zfile_hh_included_
#define _csl_common_zfile_hh_included_

#include "codesloop/common/pbuf.hh"
#include "codesloop/common/str.hh"
#ifdef __cplusplus
#include <memory>
#include <vector>

namespace csl
{
  namespace common
  {
    class zfile
    {
    public:
      bool read_file(const char * filename);
      bool read_zfile(const char * filename);

      bool write_file(const char * filename);
      bool write_zfile(const char * filename);

      uint64_t get_size();
      uint64_t get_size_const() const;

      uint64_t get_zsize();
      uint64_t get_zsize_const() const;

      bool get_data(unsigned char * dta);
      bool get_data_const(unsigned char * dta) const;
      bool get_data(pbuf & dta);
      bool get_data_const(pbuf & dta) const;
      const pbuf & get_data();

      bool get_zdata(unsigned char * dta);
      bool get_zdata_const(unsigned char * dta) const;
      bool get_zdata(pbuf & dta);
      bool get_zdata_const(pbuf & dta) const;
      const pbuf & get_zdata();

      bool put_data(const unsigned char * dta, uint64_t len);
      bool put_data(const pbuf & buf);

      bool put_zdata(const unsigned char * dta, uint64_t len);
      bool put_zdata(const pbuf & dta);

      bool dump_data(common::str & str);
      bool dump_zdata(common::str & str);

      unsigned char * get_buff();
      unsigned char * get_zbuff();

      bool drop_data();
      bool drop_zdata();

      void custom_zlib_allocator(bool yesno);
      void init_custom_memory(bool yesno);

      bool operator==(const zfile & other) const;

      zfile();
      ~zfile();

      zfile(const zfile & other);
      zfile & operator=(const zfile & other);

      struct impl;
    private:
      friend class rd_iter;

      /* private data */
      std::auto_ptr<impl> impl_;
    };
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_zfile_hh_included_ */
