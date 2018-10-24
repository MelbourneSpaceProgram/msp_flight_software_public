#ifndef SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/util/data_types.h>

class TransmitPayload : public Message {
   public:
    virtual ~TransmitPayload() {}
    virtual uint8_t GetPayloadCode() const = 0;

   protected:
    static constexpr uint8_t kPayloadSize = 251;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_TRANSMIT_PAYLOAD_H_
