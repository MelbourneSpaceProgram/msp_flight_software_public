#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>

GetConfigurationCommand::GetConfigurationCommand()
    : LithiumResponseCommand<LithiumConfiguration>(kGetConfigurationCommandCode, NULL) {}

uint16_t GetConfigurationCommand::GetLithiumPayloadSize() const { return 0; }

uint16_t GetConfigurationCommand::GetReplyPayloadSize() const {
    return GetConfigurationCommand::kGetConfigReplyPayloadSize;
}

const byte &GetConfigurationCommand::GetCommandCode() const {
    return kGetConfigurationCommandCode;
}
