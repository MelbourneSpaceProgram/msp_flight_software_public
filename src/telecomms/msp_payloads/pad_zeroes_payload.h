#ifndef SRC_TELECOMMS_MSP_PAYLOADS_PAD_ZEROES_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_PAD_ZEROES_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>

class PadZeroesPayload : public TransmitPayload {
   public:
    PadZeroesPayload();
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_PAD_ZEROES_PAYLOAD_H_
