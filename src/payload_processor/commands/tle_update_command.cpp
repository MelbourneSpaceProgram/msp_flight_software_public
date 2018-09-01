#include <external/etl/exception.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/commands/tle_update_command.h>
#include <src/util/nanopb_utils.h>
#include <ti/sysbios/BIOS.h>
#include <src/util/nanopb_utils.h>

Mailbox_Handle TleUpdateCommand::tle_update_command_mailbox_handle;

TleUpdateCommand::TleUpdateCommand(byte* payload)
    : Command(Tle_size), new_tle(NanopbDecode(Tle)(payload)) {}

void TleUpdateCommand::SetTleUpdateCommandMailboxHandle(
    Mailbox_Handle tle_update_command_mailbox_handle) {
    TleUpdateCommand::tle_update_command_mailbox_handle =
        tle_update_command_mailbox_handle;
}

bool TleUpdateCommand::ExecuteCommand() {
    return Mailbox_post(tle_update_command_mailbox_handle, &new_tle,
                        BIOS_NO_WAIT);
}
