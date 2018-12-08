#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_CREDITS_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_CREDITS_UPLINK_H_

#include <src/payload_processor/payload_processor.h>
#include <src/payload_processor/uplinks/uplink.h>

class CreditsUplink : public Uplink {
   public:
    explicit CreditsUplink(byte* payload);
    bool ExecuteUplink();

   private:
    void Say(const char* message);
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_CREDITS_UPLINK_H_
