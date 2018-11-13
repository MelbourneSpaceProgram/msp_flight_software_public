#include <src/database/flash_memory/flash_memory_management.h>
#include <src/payload_processor/uplinks/erase_flash_uplink.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

EraseFlashUplink::EraseFlashUplink(byte* payload)
    : Uplink(EraseFlashUplinkPayload_size),
      erase_flash(NanopbDecode(EraseFlashUplinkPayload)(payload)) {}

bool EraseFlashUplink::ExecuteUplink() {
    if (erase_flash.erase_flash) {
        FlashMemoryManagement::EraseFlashMemory();
        // TODO(hugorilla): Put a throw/catch statement here in case this fails
        return true;
    }
    Log_error0("Valid flash erase command was not received.");
    return false;
}
