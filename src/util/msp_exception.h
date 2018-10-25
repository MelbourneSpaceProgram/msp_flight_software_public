#ifndef SRC_UTILS_MSP_EXCEPTION_H_
#define SRC_UTILS_MSP_EXCEPTION_H_

#include <external/etl/deque.h>
#include <external/etl/exception.h>
#include <xdc/runtime/Log.h>

#define QuoteMacro(string) "\"" string "\""
#define QuoteHyphenateMacro(a, b, c, d) Quote(#a "-" #b "-" #c "-" #d)
#define MspExceptionMacro(reason, task_id, subsystem_id, error_id,          \
                          stack_level)                                      \
    MspException(                                                           \
        ETL_ERROR_TEXT(reason, QuoteHyphenateMacro(task_id, subsystem_id,   \
                                                   error_id, stack_level)), \
        task_id, subsystem_id, error_id, stack_level, __FILE__, __LINE__)

// Usage:
// throw MspExceptionMacro(verbose_reason, task_id, subsystem_id, error_id,
// stack_level);
//
// task_id, subsystem_id, error_id are all intended to help identify the problem
// on the ground
//
// TODO(dingbenjamin): stack_level gives an indication of how many levels the
// exception has propagated

// Should fit into a single 32 bit word for nice alignment
typedef struct SerialisedException {
    uint8_t task_id;  // TODO(dingbenjamin): Retrieve this at runtime
    uint8_t subsystem_id;
    uint8_t error_id;
    uint8_t stack_level;
    uint64_t timestamp_ms;
} SerialisedException;

class MspException : public etl::exception {
   public:
    static void LogException(const MspException& e);
    // TODO(dingbenjamin): Remove once all uses of etl::exception removed
    static void LogException(etl::exception& e);
    static SerialisedException PeekException();
    static SerialisedException PopException();
    static void ClearExceptions();
    static uint16_t GetNumExceptions();
    static bool IsExceptionsFull();

    MspException(string_type reason, uint8_t task_id, uint8_t subsystem_id,
                 uint8_t error_id, uint8_t stack_level, string_type file,
                 numeric_type line);
    const SerialisedException& GetSerialisedException() const;
    uint8_t GetTaskId() const;
    uint8_t GetSubsystemId() const;
    uint8_t GetErrorId() const;
    uint8_t GetStackLevel() const;
    uint64_t GetTimestamp() const;

   private:
    static void PushException(const SerialisedException& serial_exception);
    static xdc_IArg CharToIarg(const char* string);
    static constexpr uint16_t kNumExceptionsInBuffer = 500;
    static constexpr uint8_t kInvalidExceptionId = 255;
    static etl::deque<SerialisedException, kNumExceptionsInBuffer> deque;

    const SerialisedException serialised_exception;
};

#endif  // SRC_UTILS_MSP_EXCEPTION_H_
