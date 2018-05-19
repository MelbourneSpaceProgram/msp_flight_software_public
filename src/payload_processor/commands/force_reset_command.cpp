#include <src/payload_processor/commands/force_reset_command.h>
#include <ti/devices/msp432p4xx/driverlib/reset.h>

ForceResetCommand::ForceResetCommand() : Command(kForceResetCommandLength) {}

bool ForceResetCommand::ExecuteCommand() {
    ResetCtl_initiateHardResetWithSource(
        RESET_SRC_0);  // choice of reset flag 0 is arbitrary
}
