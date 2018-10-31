#include <src/payload_processor/uplinks/query_sections_uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/msp_payloads/query_sections_downlink_payload.h>
#include <src/util/nanopb_utils.h>

QuerySectionsUplink::QuerySectionsUplink(byte* payload,
                                         PayloadProcessor* payload_processor)
    : Uplink(QuerySectionsUplinkPayload_size),
      queried(NanopbDecode(QuerySectionsUplinkPayload)(payload)),
      payload_processor(payload_processor) {}

bool QuerySectionsUplink::ExecuteUplink() {
    if (queried.queried) {
        PayloadSectionManager* section_manager =
            payload_processor->GetPayloadSectionManager();
        Lithium* lithium = Lithium::GetInstance();
        QuerySectionsDownlinkPayload query(section_manager->GetBufferStatus());
        TransmitCommand section_buffer_query(&query);
        return lithium->DoCommand(&section_buffer_query);
    }
    return false;
}
