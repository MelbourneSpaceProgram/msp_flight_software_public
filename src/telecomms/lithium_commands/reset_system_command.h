#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_RESET_SYSTEM_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_RESET_SYSTEM_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>

class ResetSystemCommand : public LithiumCommand {
   public:
    ResetSystemCommand();
    uint16_t GetLithiumPayloadSize() const;
    const byte &GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;

   private:
    static const byte kResetSystemCommandCode = 0x02;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_RESET_SYSTEM_COMMAND_H_
