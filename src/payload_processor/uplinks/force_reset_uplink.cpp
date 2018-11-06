#include <external/nanopb/pb_decode.h>
#include <src/payload_processor/uplinks/force_reset_uplink.h>
#include <src/util/nanopb_utils.h>
#include <ti/devices/msp432e4/driverlib/sysctl.h>
#include <xdc/runtime/Log.h>

ForceResetUplink::ForceResetUplink(byte* payload)
    : Uplink(kForceResetUplinkLength),
      reset_enable(NanopbDecode(SystemResetUplinkPayload)(payload)) {}

bool ForceResetUplink::ExecuteUplink() {
    if (kResetUplinkCodeKey == reset_enable.reset_enable) {
        ResetManagement::ResetSystemWithMessage(kForceResetCommandExecuted);
        return true;
    } else {
        Log_error0("Reset command has failed.");
        return false;
    }
}
