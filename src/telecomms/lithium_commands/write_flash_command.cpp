#include <src/telecomms/lithium_commands/lithium_command_codes.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_md5.h>

WriteFlashCommand::WriteFlashCommand(LithiumMd5 *md5_message)
    : LithiumCommand(kWriteFlashCommandCode, md5_message),
      md5_message(md5_message) {}

uint16_t WriteFlashCommand::GetLithiumPayloadSize() const {
    return md5_message->GetSerialisedSize();
}

uint16_t WriteFlashCommand::GetReplyPayloadSize() const { return 0; }

const byte &WriteFlashCommand::GetCommandCode() const {
    return kWriteFlashCommandCode;
}
