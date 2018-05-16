#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/tle_update_command.h>

Mailbox_Handle TleUpdateCommand::tle_update_command_mailbox_handle;

TleUpdateCommand::TleUpdateCommand(byte* payload,
                                   byte command_argument_start_index)
    : Command(kTleUpdateCommandArgumentLength) {
    pb_istream_t stream =
        pb_istream_from_buffer(&payload[command_argument_start_index],
                               kTleUpdateCommandArgumentLength);

    bool status = pb_decode(&stream, Tle_fields, &new_tle);

    if (!status) {
        etl::exception e("pb_decode of new TLE failed", __FILE__, __LINE__);
        throw e;
    }
}

void TleUpdateCommand::SetTleUpdateCommandMailboxHandle(
    Mailbox_Handle tle_update_command_mailbox_handle) {
    TleUpdateCommand::tle_update_command_mailbox_handle =
        tle_update_command_mailbox_handle;
}

bool TleUpdateCommand::ExecuteCommand() {
    bool tle_update_command_delivered =
        Mailbox_post(tle_update_command_mailbox_handle, &new_tle, BIOS_NO_WAIT);
    return tle_update_command_delivered;
}
