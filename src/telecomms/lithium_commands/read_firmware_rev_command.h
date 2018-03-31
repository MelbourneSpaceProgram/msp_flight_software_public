#ifndef SRC_TELECOMMS_LITHIUM_COMMANDS_READ_FIRMWARE_REV_COMMAND_H_
#define SRC_TELECOMMS_LITHIUM_COMMANDS_READ_FIRMWARE_REV_COMMAND_H_

#include <src/telecomms/lithium_commands/lithium_command.h>

class ReadFirmwareRevCommand : public LithiumCommand {
   public:
    ReadFirmwareRevCommand();
    uint16_t GetLithiumPayloadSize() const;
    const byte &GetCommandCode() const;
    uint16_t GetReplyPayloadSize() const;
};

#endif  //  SRC_TELECOMMS_LITHIUM_COMMANDS_READ_FIRMWARE_REV_COMMAND_H_
