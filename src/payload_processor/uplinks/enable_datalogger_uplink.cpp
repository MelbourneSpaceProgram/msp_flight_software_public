#include <src/payload_processor/uplinks/enable_datalogger_uplink.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>
#include <src/sensors/runnable_system_health_check.h>

EnableDataloggerUplink::EnableDataloggerUplink(byte* payload)
    : Uplink(EnableDataloggerUplinkPayload_size),
      datalogger_enabled(NanopbDecode(EnableDataloggerUplinkPayload)(payload)) {}

bool EnableDataloggerUplink::ExecuteUplink() {
    if(datalogger_enabled.datalogger_enabled) {
        RunnableSystemHealthCheck::EnableDatalogger(true);
        Log_info0("Datalogger was enabled.");
    } else {
        RunnableSystemHealthCheck::EnableDatalogger(false);
        Log_info0("Datalogger was disabled.");
    }
    return true;
}
