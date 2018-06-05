#include <src/telecomms/lithium_commands/get_configuration_command.h>

const byte GetConfigurationCommand::kGetConfigurationCommandCode;

GetConfigurationCommand::GetConfigurationCommand()
    : LithiumCommand(kGetConfigurationCommandCode, NULL) {}

uint16_t GetConfigurationCommand::GetLithiumPayloadSize() const { return 0; }

uint16_t GetConfigurationCommand::GetReplyPayloadSize() const {
    return GetConfigurationCommand::kGetConfigReplyPayloadSize;
}

const byte &GetConfigurationCommand::GetCommandCode() const {
    return kGetConfigurationCommandCode;
}
