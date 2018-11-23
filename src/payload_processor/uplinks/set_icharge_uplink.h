#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_SET_ICHARGE_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_SET_ICHARGE_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>
#include <src/messages/SetIChargePayload.pb.h>

class SetIChargeUplink : public Uplink {
   public:
    SetIChargeUplink(byte* payload);
    bool ExecuteUplink();

   private:
    SetIChargePayload decoded_command;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_UPLINKS_SET_ICHARGE_UPLINK_H_
