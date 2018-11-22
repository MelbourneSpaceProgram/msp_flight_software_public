#ifndef SRC_TELECOMMS_MSP_PAYLOADS_LIMP_MODE_BEACON_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_LIMP_MODE_BEACON_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>
#include <src/util/data_types.h>
#include <string>

class LimpModeBeaconPayload : public TransmitPayload {
   public:
    LimpModeBeaconPayload();
    SerialisedMessage SerialiseTo(byte* serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

   private:
    static std::string kLimpModeMessage;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_LIMP_MODE_BEACON_PAYLOAD_H_
