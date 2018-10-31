#include <src/payload_processor/uplinks/section_uplink.h>
#include <src/util/nanopb_utils.h>

SectionUplink::SectionUplink(byte* payload, PayloadProcessor* payload_processor)
    : Uplink(SectionUplinkPayload_size),
      uplink_section_command(NanopbDecode(SectionUplinkPayload)(payload)),
      payload_processor(payload_processor) {}

bool SectionUplink::ExecuteUplink() {
    PayloadSectionManager* section_manager =
        payload_processor->GetPayloadSectionManager();

    return section_manager->AddSectionToBuffer(
        uplink_section_command.command_code, uplink_section_command.start_index,
        uplink_section_command.end_index,
        static_cast<byte*>(uplink_section_command.section.bytes));
}
