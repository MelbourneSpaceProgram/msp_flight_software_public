#include <external/nanopb/pb_decode.h>
#include <src/config/satellite.h>
#include <src/payload_processor/uplinks/lithium_set_pa_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

LithiumSetPaUplink::LithiumSetPaUplink(byte* payload)
    : Uplink(LithiumFastPaUplinkPayload_size),
      pa_level(NanopbDecode(LithiumFastPaUplinkPayload)(payload)) {}

bool LithiumSetPaUplink::ExecuteUplink() {
    Lithium* lithium = Lithium::GetInstance();
    FastPaCommand command = FastPaCommand(pa_level.pa_level);

    if (lithium->DoCommand(&command)) {
        return true;
    } else {
        Log_error0("Lithium power amplifier level could not be set.");
        return false;
    }
}
