#ifndef COMMANDS_LITHIUM_ENABLE_UPLINK_H_
#define COMMANDS_LITHIUM_ENABLE_UPLINK_H_

#include <external/etl/array.h>
#include <src/messages/LithiumEnableUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class LithiumEnableUplink : public Uplink {
   public:
    LithiumEnableUplink(byte* payload);
    bool ExecuteUplink();

   private:
    LithiumEnableUplinkPayload lithium_state;
    static constexpr byte kLithiumEnable = 0x01;
    static constexpr byte kLithiumDisable = 0x00;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_ENABLE_UPLINK_H_
