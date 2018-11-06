#ifndef SRC_TELECOMMS_MSP_PAYLOADS_EXCEPTIONS_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_EXCEPTIONS_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/msp_exception.h>

class ExceptionsPayload : public TransmitPayload {
   public:
    explicit ExceptionsPayload(uint8_t error_id);
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

   private:
    static constexpr uint16_t kSerialisedSize = 4 * sizeof(uint8_t) *
                                                sizeof(uint64_t) *
                                                MspException::kNumEachException;
	const uint8_t error_id;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_EXCEPTIONS_PAYLOAD_H_
