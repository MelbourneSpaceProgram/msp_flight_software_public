#ifndef SRC_PAYLOAD_PROCESSOR_COMMANDS_TEST_UPLINK_H_
#define SRC_PAYLOAD_PROCESSOR_COMMANDS_TEST_UPLINK_H_

#include <external/etl/array.h>
#include <src/payload_processor/uplinks/uplink.h>

class TestUplink : public Uplink {
   public:
    TestUplink(byte* payload);
    bool ExecuteUplink();
    static constexpr byte kTestValue1 = 0xFF;
    static constexpr byte kTestValue2 = 0xA5;

   private:
    static constexpr uint8_t kTestUplinkArgumentLength = 2;
    etl::array<byte, kTestUplinkArgumentLength> command_arguments;
};

#endif  //  SRC_PAYLOAD_PROCESSOR_COMMANDS_TEST_UPLINK_H_
