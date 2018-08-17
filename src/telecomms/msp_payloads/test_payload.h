#ifndef SRC_TELECOMMS_MSP_PAYLOADS_TEST_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_TEST_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>

class TestPayload : public TransmitPayload {
   public:
    TestPayload();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

   private:
    static const char* kTestMessage;
    // Padding is not implemented for the test message so that results can be
    // directly replicated using the Lithium Configuration Tool GUI and checksum
    // easily unit tested. All standard MSP packets are 251 bytes in length
    static const uint8_t kTestMessageSize = 56;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_TEST_PAYLOAD_H_
