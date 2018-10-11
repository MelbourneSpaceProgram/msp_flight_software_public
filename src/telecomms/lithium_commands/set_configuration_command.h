#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_SET_CONFIGURATION_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_SET_CONFIGURATION_COMMAND_H_

#include <src/telecomms/lithium_commands/set_config_message.h>
#include <src/telecomms/lithium_commands/lithium_command.h>

class SetConfigurationCommand : public LithiumCommand {
   public:
    SetConfigurationCommand(LithiumConfiguration LithiumConfig);
    uint16_t GetLithiumPayloadSize() const;
    const byte &GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;

   private:
    SetConfigurationMessage set_configuration_message;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_SET_CONFIGURATION_COMMAND_H_
