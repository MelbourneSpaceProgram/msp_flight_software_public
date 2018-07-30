#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_FAST_PA_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_FAST_PA_COMMAND_H_

#include <src/telecomms/lithium_commands/fast_pa_level.h>
#include <src/telecomms/lithium_commands/lithium_command.h>

class FastPaCommand : public LithiumCommand {
   public:
    FastPaCommand(uint8_t power_amp_level);
    uint16_t GetLithiumPayloadSize() const;
    const byte &GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;

   private:
    static const byte kFastPaCommandCode = 0x20;
    FastPaLevel fast_power_amp_level;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_FAST_PA_COMMAND_H_
