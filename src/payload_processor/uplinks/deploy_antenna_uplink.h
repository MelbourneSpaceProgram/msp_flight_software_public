#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_ANTENNA_DEPLOY_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_ANTENNA_DEPLOY_UPLINK_H_

#include <src/messages/DeployAntennaUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class DeployAntennaUplink : public Uplink {
   public:
    explicit DeployAntennaUplink(byte* payload);
    bool ExecuteUplink();

   private:
    DeployAntennaUplinkPayload deploy_antenna;
    static constexpr uint32_t kDeployUplinkCode = 0x01234567;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_ANTENNA_DEPLOY_UPLINK_H_
