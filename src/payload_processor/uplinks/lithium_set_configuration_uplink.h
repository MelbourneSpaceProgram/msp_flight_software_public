#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_SET_CONFIGURATION_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_SET_CONFIGURATION_UPLINK_H_

#include <src/messages/LithiumConfigurationPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_configuration.h>

class LithiumSetConfigurationUplink : public Uplink {
   public:
    explicit LithiumSetConfigurationUplink(byte* payload);
    bool ExecuteUplink();

   private:
    static constexpr uint8_t kLithiumSetConfigurationUplinkArgumentLength =
        LithiumConfigurationPayload_size;
    LithiumConfigurationPayload decoded_payload;
};
#endif  // SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_SET_CONFIGURATION_UPLINK_H_
