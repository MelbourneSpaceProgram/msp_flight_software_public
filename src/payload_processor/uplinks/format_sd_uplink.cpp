#include <src/database/sd_card.h>
#include <src/payload_processor/uplinks/format_sd_uplink.h>
#include <src/sensors/runnable_system_health_check.h>
#include <src/util/msp_exception.h>
#include <src/util/nanopb_utils.h>
#include <src/util/task_utils.h>

FormatSdUplink::FormatSdUplink(byte* payload)
    : Uplink(FormatSdUplinkPayload_size),
      format_sd_payload(NanopbDecode(FormatSdUplinkPayload)(payload)),
      sd_index(format_sd_payload.sd_index) {}

bool FormatSdUplink::ExecuteUplink() {
    // TODO(dingbenjamin): Validate the index is valid

    // TODO(dingbenjamin): Choose which SD card to format, and error handling if
    // the SD card is currently open
    try {
        Log_info0("Disabling data logger temporarily");
        RunnableSystemHealthCheck::EnableDatalogger(false);
        // Wait for the datalogger to finish an iteration
        TaskUtils::SleepMilli(
            kHealthCheckPeriodMs * 10);
        SdCard::GetInstance()->Format();
    } catch (etl::exception& e) {
        Log_info0("Re-enabling data logger");
        RunnableSystemHealthCheck::EnableDatalogger(true);
        MspException::LogException(e);
        Log_error0("Format SD failed");
        return false;
    }
    Log_info0("Re-enabling data logger");
    RunnableSystemHealthCheck::EnableDatalogger(true);
    return true;
}
