#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_SET_PA_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_SET_PA_UPLINK_H_

#include <src/messages/LithiumFastPaUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class LithiumSetPaUplink : public Uplink {
   public:
    LithiumSetPaUplink(byte* payload);
    bool ExecuteUplink();

   private:
    LithiumFastPaUplinkPayload pa_level;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_SET_PA_UPLINK_H_
