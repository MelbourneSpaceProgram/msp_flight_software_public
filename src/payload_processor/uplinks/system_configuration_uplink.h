#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_SYSTEM_CONFIGURATION_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_SYSTEM_CONFIGURATION_UPLINK_H_

#include <src/messages/SystemConfigurationUplinkPayload.pb.h>
#include <src/util/reset_management.h>
#include <src/payload_processor/uplinks/uplink.h>

class SystemConfigurationUplink : public Uplink {
   public:
    explicit SystemConfigurationUplink(byte* payload);
    bool ExecuteUplink();

   private:
	SystemConfigurationUplinkPayload decoded_payload;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_SYSTEM_CONFIGURATION_UPLINK_H_
