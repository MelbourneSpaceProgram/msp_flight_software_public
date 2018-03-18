#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_WRITE_FLASH_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_WRITE_FLASH_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_md5.h>

class WriteFlashCommand : public LithiumCommand {
   public:
    WriteFlashCommand(LithiumMd5 *md5_message);
    uint16_t GetLithiumPayloadSize() const;
    const byte &GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;

   private:
    LithiumMd5 *md5_message;
};

#endif  // SRC_TELECOMMS_LITHIUM_COMMANDS_WRITE_FLASH_COMMAND_H_
