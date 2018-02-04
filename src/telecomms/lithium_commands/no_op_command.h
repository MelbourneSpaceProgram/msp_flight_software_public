#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_NO_OP_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_NO_OP_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>

class NoOpCommand : public LithiumCommand {
   public:
    NoOpCommand();
    uint16_t GetLithiumPayloadSize() const;

   private:
    static const byte kNoOpCommandCode = 0x01;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_NO_OP_COMMAND_H_