#ifndef SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PACKET_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PACKET_H_

#include <src/messages/message.h>

class TransmitPayload;
class SerialisedMessageBuilder;

class TransmitPacket : public Message {
   public:
    explicit TransmitPacket(TransmitPayload *transmit_payload);
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    void EncodeWithFec(byte* buffer,
                       SerialisedMessageBuilder* builder) const;

   private:
    static constexpr uint8_t kMspHeaderSize = 4;
    uint8_t tx_count;
    uint8_t rx_count;
    uint8_t command_success_count;
    TransmitPayload *transmit_payload;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PACKET_H_
