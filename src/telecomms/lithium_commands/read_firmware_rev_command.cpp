#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/read_firmware_rev_command.h>

ReadFirmwareRevCommand::ReadFirmwareRevCommand()
    : LithiumCommand(kReadFirmwareRevCommandCode, NULL) {}

uint16_t ReadFirmwareRevCommand::GetLithiumPayloadSize() const { return 0; }

const byte &ReadFirmwareRevCommand::GetCommandCode() const {
    return kReadFirmwareRevCommandCode;
}

uint16_t ReadFirmwareRevCommand::GetReplyPayloadSize() const {
    return LithiumCommand::kNoPayload;
}
