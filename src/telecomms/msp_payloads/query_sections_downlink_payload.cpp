#include <src/messages/serialised_message_builder.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/msp_payloads/query_sections_downlink_payload.h>
#include <src/util/message_codes.h>
#include <xdc/runtime/Log.h>

QuerySectionsDownlinkPayload::QuerySectionsDownlinkPayload(
    etl::vector<std::pair<uint16_t, uint16_t>, Lithium::kMaxReceivedUplinkSize>
        empty_locations)
    : empty_locations(empty_locations) {}

SerialisedMessage QuerySectionsDownlinkPayload::SerialiseTo(
    byte* serial_buffer) const {
    SerialisedMessageBuilder builder(serial_buffer, GetSerialisedSize());
    try {
        for (uint16_t i = 0; i < empty_locations.size(); i++) {
            builder.AddData<uint16_t>(empty_locations[i].first);
            builder.AddData<uint16_t>(empty_locations[i].second);
        }
    } catch (etl::exception& e) {
        Log_error0(
            "Builder overflow in uplink section query, likely to be too many "
            "non-contiguous empty locations");
    }
    builder.PadZeroes();
    return builder.Build();
}

uint16_t QuerySectionsDownlinkPayload::GetSerialisedSize() const {
    return kMaxNumPairs * sizeof(uint16_t);
}

uint8_t QuerySectionsDownlinkPayload::GetPayloadCode() const {
    return kQuerySectionsPayloadCode;
}
