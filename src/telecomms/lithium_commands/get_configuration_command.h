#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>

class GetConfigurationCommand : public LithiumCommand {
   public:
    GetConfigurationCommand();
    uint16_t GetLithiumPayloadSize() const;

   private:
    static const byte kGetConfigurationCommandCode = 0x05;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_
