#include <external/nanopb/pb_decode.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>
#include <src/payload_processor/uplinks/set_boot_state_uplink.h>
#include <src/util/nanopb_utils.h>
#include <src/util/reset_management.h>
#include <xdc/runtime/Log.h>

SetBootStateUplink::SetBootStateUplink(byte* payload)
    : Uplink(SetBootStatePayload_size),
      decoded_command(NanopbDecode(SetBootStatePayload)(payload)) {}

bool SetBootStateUplink::ExecuteUplink() {
    ResetMessage message =
        static_cast<ResetMessage>(decoded_command.boot_state);
    if (message < ResetMessage::kNumResetMessages) {
        ResetManagement::ResetSystemWithMessage(message);
        return false;
    }
    return false;
}
