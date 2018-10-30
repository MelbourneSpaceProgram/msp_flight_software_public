#include <src/payload_processor/uplinks/lithium_enable_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

LithiumEnableUplink::LithiumEnableUplink(byte* payload)
    : Uplink(LithiumEnableUplinkPayload_size),
      lithium_state(NanopbDecode(LithiumEnableUplinkPayload)(payload)) {}

bool LithiumEnableUplink::ExecuteUplink() {
    Lithium* lithium = Lithium::GetInstance();
    if (lithium_state.should_transmit == kLithiumEnable) {
        lithium->SetTransmitEnabled(true);
        Log_info0("Enabling Lithium transmit");
    } else if (lithium_state.should_transmit == kLithiumDisable) {
        lithium->SetTransmitEnabled(false);
        Log_info0("Disabling Lithium transmit");
    } else {
        Log_error0("Invalid argument for Lithium Enable Uplink");
        return false;
    }
    return true;
}
