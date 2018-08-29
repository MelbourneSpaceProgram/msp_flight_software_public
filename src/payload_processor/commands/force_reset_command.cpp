#include <src/payload_processor/commands/force_reset_command.h>
#include <ti/devices/msp432e4/driverlib/sysctl.h>
#include <external/nanopb/pb_decode.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

ForceResetCommand::ForceResetCommand(byte* payload)
    : Command(kForceResetCommandLength),
      reset_enable(NanopbDecode(SystemResetCommand)(payload)) {}

bool ForceResetCommand::ExecuteCommand() {
    if (kResetCommandCode == reset_enable.reset_enable) {
        SysCtlReset();
        return true;
    } else {
        Log_error0("Reset command has failed.");
        return false;
    }
}
