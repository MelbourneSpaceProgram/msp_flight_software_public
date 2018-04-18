#include <external/nanopb/pb_decode.h>
#include <src/payload_processor/commands/lithium_enable_command.h>
#include <src/telecomms/lithium.h>
#include <xdc/runtime/Log.h>

LithiumEnableCommand::LithiumEnableCommand(byte* payload)
    : Command(kLithiumEnableCommandArgumentLength), lithium_state() {
    pb_istream_t stream =
        pb_istream_from_buffer(payload, LithiumOnOffState_size);
    if (!pb_decode(&stream, LithiumOnOffState_fields, &lithium_state)) {
        etl::exception e("Lithium Enable Decode Failed", __FILE__, __LINE__);
        throw e;
    }
}

bool LithiumEnableCommand::ExecuteCommand() {
    Lithium* lithium = Lithium::GetInstance();
    if (lithium_state.shouldTransmit == kLithiumEnable) {
        lithium->SetTransmitEnabled(true);
        Log_info0("Enabling Lithium transmit");
    } else if (lithium_state.shouldTransmit == kLithiumDisable) {
        lithium->SetTransmitEnabled(false);
        Log_info0("Disabling Lithium transmit");
    } else {
        Log_error0("Invalid argument for Lithium Enable Command");
        return false;
    }
    return true;
}
