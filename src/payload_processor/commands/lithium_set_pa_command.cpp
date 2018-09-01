#include <external/nanopb/pb_decode.h>
#include <src/config/satellite.h>
#include <src/payload_processor/commands/lithium_set_pa_command.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/runnable_beacon.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

LithiumSetPaCommand::LithiumSetPaCommand(byte* payload)
    : Command(LithiumFastPaCommandPayload_size),
      pa_level(NanopbDecode(LithiumFastPaCommandPayload)(payload)) {}

bool LithiumSetPaCommand::ExecuteCommand() {
    Lithium* lithium = Lithium::GetInstance();
    FastPaCommand command = FastPaCommand(pa_level.pa_level);

    if (lithium->DoCommand(&command)) {
        return true;
    } else {
        Log_error0("Lithium power amplifier level could not be set.");
        return false;
    }
}
