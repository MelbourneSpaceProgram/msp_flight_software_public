#ifndef SRC_TELECOMMS_MSP_PAYLOADS_NUM_EXCEPTIONS_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_NUM_EXCEPTIONS_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/msp_exception.h>

class NumExceptionsPayload : public TransmitPayload {
   public:
    NumExceptionsPayload();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

   private:
    static constexpr uint16_t kSerialisedSize =
        sizeof(uint8_t) * MspException::kNumExceptionTypes;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_NUM_EXCEPTIONS_PAYLOAD_H_
