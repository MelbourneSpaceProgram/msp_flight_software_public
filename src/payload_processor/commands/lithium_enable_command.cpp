#include <src/payload_processor/commands/lithium_enable_command.h>
#include <src/telecomms/lithium.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

LithiumEnableCommand::LithiumEnableCommand(byte* payload)
    : Command(LithiumEnableCommandPayload_size),
      lithium_state(NanopbDecode(LithiumEnableCommandPayload)(payload)) {}

bool LithiumEnableCommand::ExecuteCommand() {
    Lithium* lithium = Lithium::GetInstance();
    if (lithium_state.should_transmit == kLithiumEnable) {
        lithium->SetTransmitEnabled(true);
        Log_info0("Enabling Lithium transmit");
    } else if (lithium_state.should_transmit == kLithiumDisable) {
        lithium->SetTransmitEnabled(false);
        Log_info0("Disabling Lithium transmit");
    } else {
        Log_error0("Invalid argument for Lithium Enable Command");
        return false;
    }
    return true;
}
