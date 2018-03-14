#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/util/data_types.h>
#include <test_runners/lithium_hardware_tests.h>
#include <test_runners/unity.h>

// RunnableLithiumListener MUST be initialised to pass these
void TestNoOpHardware() {
#ifdef LITHIUM_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    NoOpCommand test_command;
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&test_command));
}

void TestGetConfigHardware() {
#ifdef LITHIUM_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    GetConfigurationCommand config_command;
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&config_command));
}

void TestTransmitAckHardware() {
#ifdef LITHIUM_TESTS_IGNORED
    TEST_IGNORE_MESSAGE("Hardware test ignored");
#endif
    TestPayload test_payload;
    TransmitCommand transmit_command(&test_payload, 0x67, 0x61, 0x62);
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&transmit_command));
}
