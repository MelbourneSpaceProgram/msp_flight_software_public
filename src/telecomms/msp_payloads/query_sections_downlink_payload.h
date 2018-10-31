#ifndef SRC_TELECOMMS_MSP_PAYLOADS_SECTION_BUFFER_QUERY_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_SECTION_BUFFER_QUERY_PAYLOAD_H_

#include <external/etl/vector.h>
#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <utility>

class Lithium;

class QuerySectionsDownlinkPayload : public TransmitPayload {
   public:
    explicit QuerySectionsDownlinkPayload(
        etl::vector<std::pair<uint16_t, uint16_t>,
                    Lithium::kMaxReceivedUplinkSize>
            empty_locations);
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

   private:
    etl::vector<std::pair<uint16_t, uint16_t>, Lithium::kMaxReceivedUplinkSize>
        empty_locations;
    static constexpr uint8_t kMaxNumPairs = 50;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_SECTION_BUFFER_QUERY_PAYLOAD_H_
