#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_DOWNLINK_CONFIGURATION_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_DOWNLINK_CONFIGURATION_UPLINK_H_

#include <src/messages/LithiumConfigurationPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class LithiumGetConfigurationUplink : public Uplink {
   public:
    LithiumGetConfigurationUplink();
    bool ExecuteUplink();

   private:
    static constexpr uint8_t kLithiumGetConfigurationUplinkArgumentLength = 0;
};
#endif  // SRC_PAYLOAD_PROCESSOR_UPLINKS_LITHIUM_DOWNLINK_CONFIGURATION_UPLINK_H_
