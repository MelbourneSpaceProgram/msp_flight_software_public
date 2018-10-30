#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_UPLINK_H_

#include <src/messages/SystemResetUplinkPayload.pb.h>
#include <src/util/reset_management.h>
#include <src/payload_processor/uplinks/uplink.h>

class ForceResetUplink : public Uplink {
   public:
    explicit ForceResetUplink(byte* payload);
    bool ExecuteUplink();

   private:
    SystemResetUplinkPayload reset_enable;
    static constexpr uint8_t kForceResetUplinkLength = SystemResetUplinkPayload_size;
    static constexpr uint32_t kResetUplinkCodeKey = 0xfee1dead;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_FORCE_RESET_UPLINK_H_
