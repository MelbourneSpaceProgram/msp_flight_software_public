#ifndef SRC_UTILS_ETL_UTILS_H_
#define SRC_UTILS_ETL_UTILS_H_

#include <external/etl/exception.h>
#include <xdc/runtime/Log.h>

class EtlUtils {
   public:
    static void LogException(etl::exception& e);

   private:
    static xdc_IArg CharToIarg(const char* string);
};

#endif  // SRC_UTILS_ETL_UTILS_H_
