#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>

class GetConfigurationCommand : public LithiumCommand {
   public:
    GetConfigurationCommand();
    uint16_t GetLithiumPayloadSize() const;
    const byte &GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;

   private:
    static const uint16_t kGetConfigReplyPayloadSize = 34;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_
