#include <xdc/runtime/Log.h>   // For Log_warning1("Warning number #%d", 4); things
#include <xdc/runtime/Diags.h> // For Log_print0(Diags_USER1, "hello"); things.

#define log_print xdc_runtime_Log_print0
#define log_error xdc_runtime_Log_error0
#define log_warning xdc_runtime_Log_warning0
#define log_info xdc_runtime_Log_info0
#define log_put xdc_runtime_Log_put0
#define log_write xdc_runtime_Log_write0
