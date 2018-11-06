#ifndef SRC_UTILS_MSP_EXCEPTION_H_
#define SRC_UTILS_MSP_EXCEPTION_H_

#include <external/etl/deque.h>
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
          task_name((*static_cast<const char**>(task_stats.env))[0]),
          task_priority(task_stats.priority),
          task_stack_used(task_stats.used),
          exception_param_1(0),
          exception_param_2(0) {}

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
    static void LogException(MspException& e, CatchId catch_id);
    static void LogException(const MspException& e);
    // TODO(dingbenjamin): Remove once all uses of etl::exception removed
    static void LogException(etl::exception& e);
    static SerialisedException PeekException();
    static SerialisedException PopException();
    static void ClearExceptions();
    static uint16_t GetNumExceptions();
    static bool IsExceptionsFull();

    MspException(string_type reason, uint8_t error_id, string_type file,
                 numeric_type line, uint8_t exception_param_1 = 0,
                 uint8_t exception_param_2 = 0);
    const SerialisedException& GetSerialisedException() const;
    uint8_t GetErrorId() const;
    uint64_t GetTimestamp() const;

   private:
    static void PushException(const SerialisedException& serial_exception);
    static xdc_IArg CharToIarg(const char* string);
    static constexpr uint16_t kNumExceptionsInBuffer = 500;
    static constexpr uint8_t kInvalidExceptionId = 255;
    static etl::deque<SerialisedException, kNumExceptionsInBuffer> deque;

    SerialisedException serialised_exception;
};

#endif  // SRC_UTILS_MSP_EXCEPTION_H_
