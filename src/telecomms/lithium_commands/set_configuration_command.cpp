#include <src/telecomms/lithium_commands/set_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command_codes.h>

SetConfigurationCommand::SetConfigurationCommand(LithiumConfiguration lithium_config)
    : LithiumCommand(kSetConfigurationCommandCode, &set_configuration_message),
      set_configuration_message(lithium_config) {}

uint16_t SetConfigurationCommand::GetLithiumPayloadSize() const {
    return set_configuration_message.GetSerialisedSize();
}

const byte& SetConfigurationCommand::GetCommandCode() const {
    return kSetConfigurationCommandCode;
}

uint16_t SetConfigurationCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}
