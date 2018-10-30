#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_ENABLE_DATALOGGER_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_ENABLE_DATALOGGER_UPLINK_H_

#include <src/messages/EnableDataloggerUplinkPayload.pb.h>
#include <src/payload_processor/uplinks/uplink.h>

class EnableDataloggerUplink : public Uplink {
   public:
    explicit EnableDataloggerUplink(byte* payload);
    bool ExecuteUplink();

   private:
    EnableDataloggerUplinkPayload datalogger_enabled;
};

#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_ENABLE_DATALOGGER_UPLINK_H_
