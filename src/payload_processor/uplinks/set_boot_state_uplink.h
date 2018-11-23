#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_SET_BOOT_STATE_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_SET_BOOT_STATE_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>
#include <src/messages/SetBootStatePayload.pb.h>

class SetBootStateUplink : public Uplink {
   public:
    SetBootStateUplink(byte* payload);
    bool ExecuteUplink();

   private:
    SetBootStatePayload decoded_command;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_UPLINKS_SET_BOOT_STATE_UPLINK_H_
