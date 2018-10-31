#include <src/messages/LithiumMd5Payload.pb.h>
#include <src/payload_processor/uplinks/lithium_write_flash_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/util/nanopb_utils.h>

LithiumWriteFlashUplink::LithiumWriteFlashUplink(byte* payload)
    : Uplink(kLithiumWriteFlashUplinkArgumentLength),
      md5(NanopbDecode(LithiumMd5Payload)(payload)) {}

bool LithiumWriteFlashUplink::ExecuteUplink() {
    WriteFlashCommand command(&md5);
    return Lithium::GetInstance()->DoCommand(&command);
}
