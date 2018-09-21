#include <src/util/etl_utils.h>
#include <string>

void EtlUtils::LogException(etl::exception& e) {
    Log_error3("Exception Occurred: %s\n File: %s, Line %d",
               CharToIarg(e.what()), CharToIarg(e.file_name()),
               e.line_number());
}

xdc_IArg EtlUtils::CharToIarg(const char* string) {
    int iarg = reinterpret_cast<unsigned int>(string);
    return iarg;
}
