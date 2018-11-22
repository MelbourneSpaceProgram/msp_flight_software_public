#ifndef SRC_UTILS_MSP_EXCEPTION_H_
#define SRC_UTILS_MSP_EXCEPTION_H_

#include <external/etl/exception.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <src/util/satellite_time_source.h>
#include <ti/sysbios/knl/Task.h>
#include <xdc/runtime/Log.h>

typedef struct SerialisedException {
    SerialisedException(uint8_t error_id, uint8_t catch_id,
                        Task_Stat task_stats, uint8_t exception_param_1 = 0,
                        uint8_t exception_param_2 = 0)
        : timestamp_ms(SatelliteTimeSource::GetTime().timestamp_ms),
          error_id(error_id),
          catch_id(catch_id),
          task_name(*((char*)task_stats.env)),
          task_priority(task_stats.priority),
          task_stack_used(task_stats.used),
          exception_param_1(exception_param_1),
          exception_param_2(exception_param_2) {}

    SerialisedException()
        : timestamp_ms(0),
          error_id(0),
          catch_id(0),
          task_name(0),
          task_priority(0),
          task_stack_used(0),
          exception_param_1(0),
          exception_param_2(0) {}

    uint64_t timestamp_ms;
    uint8_t error_id;
    uint8_t catch_id;
    char task_name;
    uint8_t task_priority;
    uint16_t task_stack_used;
    byte exception_param_1;
    byte exception_param_2;
} SerialisedException;

class MspException : public etl::exception {
   public:
    static constexpr uint8_t kNumEachException = 5;

    // TODO(dingbenjamin): Haven't been able to perform static compile time
    // initialisation due to some suspected linker issues, investigate and
    // remove need for Init() function
    static void Init();
    static void LogException(MspException& e, CatchId catch_id,
                             bool store_only = false);
    static void LogException(const MspException& e, bool store_only = false);
    static void LogException(etl::exception& e);
    static void ClearType(uint8_t error_id);
    static void ClearAll();
    static uint8_t GetNumType(uint8_t error_id);
    static const uint8_t* GetNumAll();
    static const SerialisedException* GetExceptions(uint8_t error_id);

    MspException(string_type reason, ErrorId error_id, string_type file,
                 numeric_type line, uint8_t exception_param_1 = 0,
                 uint8_t exception_param_2 = 0);
    const SerialisedException& GetSerialisedException() const;
    uint8_t GetErrorId() const;
    uint64_t GetTimestamp() const;

   private:
    static xdc_IArg CharToIarg(const char* string);

    // TODO(dingbenjamin): Store in flash memory
    static SerialisedException** exception_log;
    static uint8_t* num_exceptions;
    static uint8_t* log_circle_index;
    static bool initialised;

    SerialisedException serialised_exception;
};

#endif  // SRC_UTILS_MSP_EXCEPTION_H_
