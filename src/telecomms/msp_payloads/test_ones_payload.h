#ifndef SRC_TELECOMMS_MSP_PAYLOADS_TEST_PAYLOAD_ONES_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_TEST_PAYLOAD_ONES_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>

class TestOnesPayload : public TransmitPayload {
   public:
    TestOnesPayload();
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_TEST_PAYLOAD_ONES_H_
