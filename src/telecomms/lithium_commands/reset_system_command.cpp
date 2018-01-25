#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/util/data_types.h>

ResetSystemCommand::ResetSystemCommand()
    : LithiumCommand(kResetSystemCommandCode, NULL) {}

uint16_t ResetSystemCommand::GetLithiumPayloadSize() const { return 0; }
