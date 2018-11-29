#include <src/config/satellite.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_time_source.h>
#include <algorithm>
#include <string>

SerialisedException** MspException::exception_log = NULL;
uint8_t* MspException::num_exceptions = NULL;
uint8_t* MspException::log_circle_index = NULL;
bool MspException::initialised = false;

void MspException::Init() {
    exception_log = new SerialisedException*[kNumExceptionTypes];
    for (uint8_t i = 0; i < kNumExceptionTypes; i++) {
        exception_log[i] = new SerialisedException[kNumEachException];
    }

    num_exceptions = new uint8_t[kNumExceptionTypes];
    log_circle_index = new uint8_t[kNumExceptionTypes];
    for (uint8_t i = 0; i < kNumExceptionTypes; i++) {
        num_exceptions[i] = 0;
        log_circle_index[i] = 0;
    }

    initialised = true;
}

MspException::MspException(string_type reason, ErrorId error_id,
                           string_type file, numeric_type line,
                           uint8_t exception_param_1, uint8_t exception_param_2)
    : exception(reason, file, line) {
    Task_Stat task_stat;
    Task_stat(Task_self(), &task_stat);
    serialised_exception = SerialisedException(
        static_cast<uint8_t>(error_id), static_cast<uint8_t>(kUncaught),
        task_stat, exception_param_1, exception_param_2);
}

void MspException::LogException(MspException& e, CatchId catch_id,
                                bool store_only) {
    e.serialised_exception.catch_id = catch_id;
    LogException(e, store_only);
}

void MspException::LogException(const MspException& e, bool store_only) {
    if (initialised) {
        uint8_t error_id = e.serialised_exception.error_id;

        // Circular buffer
        uint8_t index = log_circle_index[error_id];
        exception_log[error_id][index] = e.serialised_exception;
        num_exceptions[error_id] = num_exceptions[error_id] >= kNumEachException
                                       ? kNumEachException
                                       : num_exceptions[error_id] + 1;
        log_circle_index[error_id] = (index + 1) % kNumEachException;
    } else {
        Log_warning0("Exception occurred but error logging is not initialised");
    }

    if (!store_only)
        Log_error3("Exception Occurred: %s\n File: %s, Line %d",
                   CharToIarg(e.what()), CharToIarg(e.file_name()),
                   e.line_number());
}

void MspException::LogTopLevelException(MspException& e, CatchId catch_id,
                                        bool store_only) {
    MspException::LogException(e, catch_id, store_only);
    if (kRebootOnTopLevelException) throw;
    TirtosUtils::SleepMilli(
        10000);  // Allow a backoff period before trying the task again
}

void MspException::ClearType(uint8_t error_id) {
    if (!initialised) {
        Log_warning0(
            "Tried to clear exceptions but error logging uninitialised");
        return;
    }

    if (error_id < kNumExceptionTypes) {
        num_exceptions[error_id] = 0;
        log_circle_index[kNumExceptionTypes] = 0;
    } else {
        throw MspException("Invalid exception index to clear",
                           kMspExceptionInvalidClearIndexFail, __FILE__,
                           __LINE__);
    }
}

void MspException::ClearAll() {
    if (!initialised) {
        Log_warning0(
            "Tried to clear exceptions but error logging uninitialised");
        return;
    }

    for (uint8_t i = 0; i < kNumExceptionTypes; ++i) {
        ClearType(i);
    }
}

// Could return uninitialised exceptions if size is less than max but that's
// fine
const SerialisedException* MspException::GetExceptions(uint8_t error_id) {
    if (error_id < kNumExceptionTypes && initialised) {
        return exception_log[error_id];
    }

    if (!initialised) {
        Log_warning0("Cannot get exceptions, logging uninitialised");
    }

    return NULL;
}

uint8_t MspException::GetNumType(uint8_t error_id) {
    if (error_id < kNumExceptionTypes && initialised) {
        return num_exceptions[error_id];
    }

    if (!initialised) {
        Log_warning0("Cannot get exceptions, logging uninitialised");
    }

    return 0;
}

const uint8_t* MspException::GetNumAll() {
    if (!initialised) {
        Log_warning0("Cannot get exceptions, logging uninitialised");
        return NULL;
    }
    return num_exceptions;
}

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
