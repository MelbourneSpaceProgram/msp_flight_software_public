#include <src/util/msp_exception.h>
#include <string>

void MspException::LogException(etl::exception& e) {
    Log_error3("Exception Occurred: %s\n File: %s, Line %d",
               CharToIarg(e.what()), CharToIarg(e.file_name()),
               e.line_number());
}

xdc_IArg MspException::CharToIarg(const char* string) {
    int iarg = reinterpret_cast<unsigned int>(string);
    return iarg;
}
