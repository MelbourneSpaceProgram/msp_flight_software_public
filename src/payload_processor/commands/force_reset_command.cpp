#include <src/payload_processor/commands/force_reset_command.h>
#include <ti/devices/msp432e4/driverlib/sysctl.h>

ForceResetCommand::ForceResetCommand() : Command(kForceResetCommandLength) {}

bool ForceResetCommand::ExecuteCommand() {
    SysCtlReset();
    return true;
}
