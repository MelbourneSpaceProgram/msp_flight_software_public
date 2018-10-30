#include <external/etl/exception.h>
#include <src/messages/Tle.pb.h>
#include <src/payload_processor/uplinks/tle_update_uplink.h>
#include <src/util/nanopb_utils.h>
#include <ti/sysbios/BIOS.h>
#include <src/util/nanopb_utils.h>

Mailbox_Handle TleUpdateUplink::tle_update_command_mailbox_handle;

TleUpdateUplink::TleUpdateUplink(byte* payload)
    : Uplink(Tle_size), new_tle(NanopbDecode(Tle)(payload)) {}

void TleUpdateUplink::SetTleUpdateUplinkMailboxHandle(
    Mailbox_Handle tle_update_command_mailbox_handle) {
    TleUpdateUplink::tle_update_command_mailbox_handle =
        tle_update_command_mailbox_handle;
}

bool TleUpdateUplink::ExecuteUplink() {
    return Mailbox_post(tle_update_command_mailbox_handle, &new_tle,
                        BIOS_NO_WAIT);
}
