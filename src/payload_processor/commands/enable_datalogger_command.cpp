#include <src/payload_processor/commands/enable_datalogger_command.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>
#include <src/system/runnable_system_health_check.h>

EnableDataloggerCommand::EnableDataloggerCommand(byte* payload)
    : Command(EnableDataloggerCommandPayload_size),
      datalogger_enabled(NanopbDecode(EnableDataloggerCommandPayload)(payload)) {}

bool EnableDataloggerCommand::ExecuteCommand() {
    if(datalogger_enabled.datalogger_enabled) {
        RunnableSystemHealthCheck::EnableDatalogger(true);
        Log_info0("Datalogger was enabled.");
    } else {
        RunnableSystemHealthCheck::EnableDatalogger(false);
        Log_info0("Datalogger was disabled.");
    }
    return true;
}
