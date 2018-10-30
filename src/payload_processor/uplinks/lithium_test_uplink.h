#ifndef COMMANDS_LITHIUM_TEST_UPLINK_H_
#define COMMANDS_LITHIUM_TEST_UPLINK_H_

#include <src/payload_processor/uplinks/uplink.h>

class LithiumTestUplink : public Uplink {
   public:
    LithiumTestUplink();
    bool ExecuteUplink();

   private:
    static constexpr uint8_t kLithiumTestUplinkArgumentLength = 1;
    static constexpr uint8_t kNumTestTransmits = 10;
};
#endif  // SRC_PAYLOAD_PROCESSOR_COMMANDS_LITHIUM_TEST_UPLINK_H_
