#ifndef SRC_PAYLOAD_PROCESSOR_UPLINKS_TERMINATE_ANTENNA_BURN_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_UPLINKS_TERMINATE_ANTENNA_BURN_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>
#include <src/messages/TerminateAntennaBurnPayload.pb.h>

class TerminateAntennaBurnUplink : public Uplink {
   public:
    TerminateAntennaBurnUplink(byte* payload);
    bool ExecuteUplink();
    static constexpr uint32_t kTerminateAntennaBurnDisableKey = 0xFFFFFFFF;
    static constexpr uint32_t kTerminateAntennaBurnEnableKey = 0xF0F0F0F0;

   private:
    TerminateAntennaBurnPayload decoded_command;
};
#endif  //  SRC_PAYLOAD_PROCESSOR_UPLINKS_TERMINATE_ANTENNA_BURN_UPLINK_H_
