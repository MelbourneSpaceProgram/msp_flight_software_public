#include <src/database/sd_card.h>
#include <src/payload_processor/commands/format_sd_command.h>
#include <src/util/nanopb_utils.h>

FormatSdCommand::FormatSdCommand(byte* payload)
    : Command(FormatSdCommandPayload_size),
      format_sd_payload(NanopbDecode(FormatSdCommandPayload)(payload)),
      sd_index(format_sd_payload.sd_index) {}

bool FormatSdCommand::ExecuteCommand() {
    // TODO(dingbenjamin): Validate the index is valid

    // TODO(dingbenjamin): Choose which SD card to format, and error handling if
    // the SD card is currently open
    SdCard::Format();
    return true;
}
