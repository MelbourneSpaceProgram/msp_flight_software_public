#ifndef SRC_DATABASE_SD_EXCEPTION_H_
#define SRC_DATABASE_SD_EXCEPTION_H_

#include <src/database/sd_card.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>

// TODO(dingbenjamin): SD Exception Macro for verbose and terse text

class SdException : public MspException {
   public:
    SdException(string_type reason, uint8_t task_id, SdErrorId error_id,
                string_type file, numeric_type line, uint8_t f_result = 0)
        : MspException(reason, task_id, kSdSubsystem, error_id, 0, file, line),
          f_result(static_cast<FResult>(f_result)) {}

    const FResult f_result;
};

#endif  // SRC_DATABASE_SD_EXCEPTION_H_
