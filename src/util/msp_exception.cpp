#include <src/util/msp_exception.h>
#include <src/util/satellite_time_source.h>
#include <string>

etl::deque<SerialisedException, MspException::kNumExceptionsInBuffer>
    MspException::deque;

MspException::MspException(string_type reason, uint8_t task_id,
                           uint8_t subsystem_id, uint8_t error_id,
                           uint8_t stack_level, string_type file,
                           numeric_type line)
    : exception(reason, file, line),
      serialised_exception({task_id, subsystem_id, error_id, stack_level,
                            SatelliteTimeSource::GetTime().timestamp_ms}) {}

void MspException::LogException(const MspException& e) {
    // Circular buffer
    if (deque.full()) deque.pop_front();
    deque.push_back(e.GetSerialisedException());

    Log_error3("Exception Occurred: %s\n File: %s, Line %d",
               CharToIarg(e.what()), CharToIarg(e.file_name()),
               e.line_number());
}

void MspException::LogException(etl::exception& e) {
    Log_error3("Exception Occurred: %s\n File: %s, Line %d",
               CharToIarg(e.what()), CharToIarg(e.file_name()),
               e.line_number());
}

SerialisedException MspException::PopException() {
    // Invalid exception
    if (deque.empty())
        return {kInvalidExceptionId, kInvalidExceptionId, kInvalidExceptionId,
                kInvalidExceptionId};

    SerialisedException popped_exception = deque.front();
    deque.pop_front();
    return popped_exception;
}

SerialisedException MspException::PeekException() { return deque.front(); }

void MspException::ClearExceptions() { deque.clear(); }

uint16_t MspException::GetNumExceptions() {
    // TODO(dingbenjamin): Check if this is in bytes or number of exceptions
    return deque.size();
}

bool MspException::IsExceptionsFull() { return deque.full(); }

xdc_IArg MspException::CharToIarg(const char* string) {
    int iarg = reinterpret_cast<unsigned int>(string);
    return iarg;
}

const SerialisedException& MspException::GetSerialisedException() const {
    return serialised_exception;
}

uint8_t MspException::GetTaskId() const { return serialised_exception.task_id; }

uint8_t MspException::GetSubsystemId() const {
    return serialised_exception.subsystem_id;
}

uint8_t MspException::GetErrorId() const {
    return serialised_exception.error_id;
}

uint8_t MspException::GetStackLevel() const {
    return serialised_exception.stack_level;
}

uint64_t MspException::GetTimestamp() const {
    return serialised_exception.timestamp_ms;
}
