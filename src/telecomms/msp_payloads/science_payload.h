#ifndef SRC_TELECOMMS_MSP_PAYLOADS_SCIENCE_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_SCIENCE_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <src/util/nanopb_utils.h>

template <typename NanopbMessageType, uint16_t NanopbMessageType_size,
          const pb_field_t* NanopbMessageType_fields>
class SciencePayload : public TransmitPayload {
   public:
    explicit SciencePayload(NanopbMessageType reading) : reading(reading) {}

    SerialisedMessage SerialiseTo(byte* serial_buffer) const {
        NanopbUtils::Encode<NanopbMessageType, NanopbMessageType_size,
                            NanopbMessageType_fields>(serial_buffer, reading);
        return SerialisedMessage(NanopbMessageType_size, serial_buffer);
    }

    uint16_t GetSerialisedSize() const { return NanopbMessageType_size; }
    uint8_t GetPayloadCode() const { return kSciencePayloadCode; }

   private:
    NanopbMessageType reading;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_SCIENCE_PAYLOAD_H_
