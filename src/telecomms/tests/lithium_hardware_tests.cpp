#include <external/etl/array.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_md5.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/util/data_types.h>
#include <test_runners/unity.h>

// RunnableLithiumListener MUST be initialised to pass these
void TestNoOpHardware() {
    if (!lithium_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    NoOpCommand test_command;
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&test_command));
}

void TestGetConfigHardware() {
    if (!lithium_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    GetConfigurationCommand config_command;
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&config_command));
}

void TestTransmitAckHardware() {
    if (!lithium_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    TestPayload test_payload;
    TransmitCommand transmit_command(&test_payload, 0x67, 0x61, 0x62);
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&transmit_command));
}

void TestWriteFlashHardware() {
    if (!lithium_flash_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    etl::array<byte, LithiumMd5::kNumMd5Bytes> md5_bytes = {
        0x9b, 0x20, 0x4f, 0xc6, 0x5f, 0x0f, 0x1e, 0x60,
        0x7f, 0xc1, 0x82, 0x89, 0x6d, 0x81, 0xc1, 0x12};
    LithiumMd5 md5_message(&md5_bytes);
    WriteFlashCommand flash_command(&md5_message);
    TEST_ASSERT(Lithium::GetInstance()->DoCommand(&flash_command));
}
