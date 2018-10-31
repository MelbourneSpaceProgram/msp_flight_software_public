#include <src/payload_processor/uplinks/clear_sections_uplink.h>
#include <src/util/nanopb_utils.h>

ClearSectionsUplink::ClearSectionsUplink(byte* payload,
                                         PayloadProcessor* payload_processor)
    : Uplink(ClearSectionsUplinkPayload_size),
      clear_buffer(NanopbDecode(ClearSectionsUplinkPayload)(payload)),
      payload_processor(payload_processor) {}

bool ClearSectionsUplink::ExecuteUplink() {
    if (clear_buffer.clear_buffer) {
        PayloadSectionManager* section_manager =
            payload_processor->GetPayloadSectionManager();
        section_manager->ClearBuffer();
        return true;
    }
    return false;
}
