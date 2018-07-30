#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_md5.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/util/data_types.h>

TEST_GROUP(Lithium) {
    void setup() {
        if (!tcom_board_available) {
            TEST_EXIT
        }
    };
};

// RunnableLithiumListener MUST be initialised to pass these
TEST(Lithium, TestNoOpHardware) {
    NoOpCommand test_command;
    CHECK(Lithium::GetInstance()->DoCommand(&test_command));
}

TEST(Lithium, TestGetConfigHardware) {
    GetConfigurationCommand config_command;
    CHECK(Lithium::GetInstance()->DoCommand(&config_command));
}

TEST(Lithium, TestTransmitAckHardware) {
    TestPayload test_payload;
    TransmitCommand transmit_command(&test_payload, 0x67, 0x61, 0x62);
    CHECK(Lithium::GetInstance()->DoCommand(&transmit_command));
}

TEST(Lithium, TestWriteFlashHardware) {
    if (!lithium_flash_test_enabled) {
        TEST_EXIT;
    }
    etl::array<byte, LithiumMd5::kNumMd5Bytes> md5_bytes = {
        0x9b, 0x20, 0x4f, 0xc6, 0x5f, 0x0f, 0x1e, 0x60,
        0x7f, 0xc1, 0x82, 0x89, 0x6d, 0x81, 0xc1, 0x12};
    LithiumMd5 md5_message(&md5_bytes);
    WriteFlashCommand flash_command(&md5_message);
    CHECK(Lithium::GetInstance()->DoCommand(&flash_command));
}

TEST(Lithium, TestFastPaHardware) {
    FastPaCommand fast_pa_command(5);
    CHECK(Lithium::GetInstance()->DoCommand(&fast_pa_command));
}
