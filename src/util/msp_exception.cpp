#include <src/util/msp_exception.h>
#include <string>

etl::deque<SerialisedException, MspException::kNumExceptionsInBuffer>
    MspException::deque;

MspException::MspException(string_type reason, uint8_t error_id,
                           string_type file, numeric_type line,
                           uint8_t exception_param_1, uint8_t exception_param_2)
    : exception(reason, file, line) {
    Task_Stat task_stat;
    Task_stat(Task_self(), &task_stat);
    serialised_exception =
        SerialisedException(error_id, static_cast<uint8_t>(kUncaught),
                            task_stat, exception_param_1, exception_param_2);
}

void MspException::LogException(MspException& e, CatchId catch_id) {
    e.serialised_exception.catch_id = catch_id;
    LogException(e);
}

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
    if (deque.empty()) return SerialisedException();
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

uint8_t MspException::GetErrorId() const {
    return serialised_exception.error_id;
}
