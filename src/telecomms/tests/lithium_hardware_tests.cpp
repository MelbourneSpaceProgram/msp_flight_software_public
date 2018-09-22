#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/telemetry_query_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_md5.h>
#include <src/telecomms/lithium_telemetry.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/util/data_types.h>

TEST_GROUP(Lithium) {
    void setup() {
        if (!kTcomBoardAvailable) {
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
    TransmitCommand transmit_command(&test_payload);
    CHECK(Lithium::GetInstance()->DoCommand(&transmit_command));
}

TEST(Lithium, TestWriteFlashHardware) {
    if (!kLithiumFlashTestEnabled) {
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
    uint8_t pa_level = 0x04;
    FastPaCommand fast_pa_command(pa_level);
    CHECK(Lithium::GetInstance()->DoCommand(&fast_pa_command));
    GetConfigurationCommand config_command;
    Lithium::GetInstance()->DoCommand(&config_command);
    LithiumConfiguration received_configuration =
        config_command.GetParsedResponse();
    CHECK_EQUAL(received_configuration.tx_power_amp_level, pa_level);
}

TEST(Lithium, TestTelemetryQueryHardware) {
    LithiumTelemetry telemetry = Lithium::GetInstance()->ReadLithiumTelemetry();
    CHECK(TelemetryQueryCommand::CheckValidTelemetry(telemetry));
}
