#ifndef SRC_TELECOMMS_MSP_PAYLOADS_LITHIUM_GET_CONFIGURATION_PAYLOAD_H_
#define SRC_TELECOMMS_MSP_PAYLOADS_LITHIUM_GET_CONFIGURATION_PAYLOAD_H_

#include <src/messages/message.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/msp_payloads/transmit_payload.h>

class LithiumGetConfigurationUplinkPayload : public TransmitPayload {
   public:
    // TODO(dingbenjamin): Consolidate all the Lithium configuration classes
    explicit LithiumGetConfigurationUplinkPayload(LithiumConfiguration config);
    SerialisedMessage SerialiseTo(byte *serial_buffer) const;
    uint16_t GetSerialisedSize() const;
    uint8_t GetPayloadCode() const;

   private:
    LithiumConfiguration config;
    static constexpr uint8_t kPayloadSize = 35;
};

#endif  // SRC_TELECOMMS_MSP_PAYLOADS_LITHIUM_GET_CONFIGURATION_PAYLOAD_H_
