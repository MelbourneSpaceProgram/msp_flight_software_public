///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2015 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef __ETL_IHASH__
#define __ETL_IHASH__

#include <stdint.h>
#include <utility>

#include "../../external/etl/error_handler.h"
#include "../../external/etl/exception.h"
#include "../../external/etl/platform.h"

///\defgroup ihash Common data for all hash type classes.
///\ingroup hash

#undef ETL_FILE
#define ETL_FILE "19"

namespace etl
{
  //***************************************************************************
  ///\ingroup hash
  /// Exception base for hashes.
  //***************************************************************************
  class hash_exception : public exception
  {
  public:

    hash_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {}
  };

  //***************************************************************************
  ///\ingroup vector
  /// Hash finalised exception.
  //***************************************************************************
  class hash_finalised : public hash_exception
  {
  public:

    hash_finalised(string_type file_name_, numeric_type line_number_)
      : hash_exception(ETL_ERROR_TEXT("ihash:finalised", ETL_FILE"A"), file_name_, line_number_)
    {}
  };

  /// For the Americans
  typedef hash_finalised hash_finalized;
}

#undef ETL_FILE

#endif
