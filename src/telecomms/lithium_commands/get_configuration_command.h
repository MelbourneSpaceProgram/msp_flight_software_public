#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/lithium_response_command.h>
#include <src/util/data_types.h>
#include <src/telecomms/lithium_configuration.h>

class GetConfigurationCommand
    : public LithiumResponseCommand<LithiumConfiguration> {
   public:
    GetConfigurationCommand();
    uint16_t GetLithiumPayloadSize() const;
    const byte& GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;
    LithiumConfiguration GetParsedResponse();

   private:
    static constexpr uint16_t kGetConfigReplyPayloadSize = 34;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_GET_CONFIGURATION_COMMAND_H_
