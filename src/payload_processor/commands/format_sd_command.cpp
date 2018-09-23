#include <src/database/sd_card.h>
#include <src/payload_processor/commands/format_sd_command.h>
#include <src/system/runnable_system_health_check.h>
#include <src/util/etl_utils.h>
#include <src/util/nanopb_utils.h>
#include <src/util/task_utils.h>

FormatSdCommand::FormatSdCommand(byte* payload)
    : Command(FormatSdCommandPayload_size),
      format_sd_payload(NanopbDecode(FormatSdCommandPayload)(payload)),
      sd_index(format_sd_payload.sd_index) {}

bool FormatSdCommand::ExecuteCommand() {
    // TODO(dingbenjamin): Validate the index is valid

    // TODO(dingbenjamin): Choose which SD card to format, and error handling if
    // the SD card is currently open
    try {
        Log_info0("Disabling data logger temporarily");
        RunnableSystemHealthCheck::EnableDatalogger(false);
        // Wait for the datalogger to finish an iteration
        TaskUtils::SleepMilli(10000);
        SdCard::Format();
    } catch (etl::exception& e) {
        Log_info0("Re-enabling data logger");
        RunnableSystemHealthCheck::EnableDatalogger(true);
        EtlUtils::LogException(e);
        Log_error0("Format SD failed");
        return false;
    }
    Log_info0("Re-enabling data logger");
    RunnableSystemHealthCheck::EnableDatalogger(true);
    return true;
}
