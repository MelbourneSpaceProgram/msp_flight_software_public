#include <src/payload_processor/uplinks/execute_sections_uplink.h>
#include <src/util/nanopb_utils.h>
#include <xdc/runtime/Log.h>

ExecuteSectionsUplink::ExecuteSectionsUplink(
    byte* payload, PayloadProcessor* payload_processor)
    : Uplink(ExecuteSectionsUplinkPayload_size),
      post_buffer(NanopbDecode(ExecuteSectionsUplinkPayload)(payload)),
      payload_processor(payload_processor) {}

bool ExecuteSectionsUplink::ExecuteUplink() {
    // TODO(dingbenjamin): Check if we are the correct length for the given
    // uplink
    PayloadSectionManager* section_manager =
        payload_processor->GetPayloadSectionManager();
    byte* section_buffer = section_manager->GetBuffer();
    uint16_t command_code =
        section_buffer[0] | (static_cast<uint16_t>(section_buffer[1] << 8));
    if (section_buffer[0] == 0 && section_buffer[1] == 0) {
        Log_warning0("Tried to execute an empty/NoCommand section buffer");
        return false;
    } else if (command_code >= static_cast<uint16_t>(kUplinksLength)) {
        Log_error1("Tried to execute an invalid command code %d", command_code);
        return false;
    } else if (post_buffer.post_buffer) {
        return payload_processor->ParseAndExecuteUplinks(
            section_manager->GetBuffer());
    }
    return false;
}
