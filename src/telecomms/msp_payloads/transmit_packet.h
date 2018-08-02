#ifndef SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PACKET_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PACKET_H_

#include <src/messages/message.h>

class TransmitPayload;

class TransmitPacket : public Message {
   public:
    TransmitPacket(TransmitPayload *transmit_payload);
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;

   private:
    static const uint8_t kMspHeaderSize = 4;
    TransmitPayload *transmit_payload;
    uint8_t tx_count;
    uint8_t rx_count;
    uint8_t command_success_count;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PACKET_H_
