#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_UPLINK_H_

#include <src/messages/LithiumBeaconPeriodUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class LithiumBeaconPeriodUplink : public Uplink {
   public:
    explicit LithiumBeaconPeriodUplink(byte* payload);
    bool ExecuteUplink();

   private:
    LithiumBeaconPeriodUplinkPayload beacon_period;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_BEACON_PERIOD_UPLINK_H_
