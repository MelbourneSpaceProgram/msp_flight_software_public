#include <src/payload_processor/payload_processor.h>
#include <test_runners/unity.h>
#include <src/telecomms/lithium.h>
#include <test_runners/payload_processor_tests.h>
#include <src/payload_processor/commands/test_command.h>

void TestPayloadProcessor(void) {
    byte payload[Lithium::kMaxReceivedSize] = {0};

    // Set the bytes necessary for 2 x test command
    payload[0] = 1;
    payload[1] = TestCommand::kTestValue1;
    payload[2] = TestCommand::kTestValue2;
    payload[3] = 1;
    payload[4] = TestCommand::kTestValue1;
    payload[5] = TestCommand::kTestValue2;

    PayloadProcessor payload_processor;
    TEST_ASSERT(payload_processor.ParseAndExecuteCommands(payload));
}
