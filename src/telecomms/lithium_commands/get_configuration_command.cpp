#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/util/data_types.h>

GetConfigurationCommand::GetConfigurationCommand()
    : LithiumCommand(kGetConfigurationCommandCode, NULL) {}

uint16_t GetConfigurationCommand::GetLithiumPayloadSize() const { return 0; }
