#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/lithium_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/set_configuration_command.h>
#include <src/telecomms/lithium_commands/telemetry_query_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_configuration.h>
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

TEST(Lithium, TestGetSetConfigHardware) {
    LithiumConfiguration lithium_config;
    lithium_config.interface_baud_rate = 0x00;
    lithium_config.tx_power_amp_level = 0x29;
    lithium_config.rx_rf_baud_rate = 0x01;
    lithium_config.tx_rf_baud_rate = 0x01;
    lithium_config.rx_modulation = 0x00;
    lithium_config.tx_modulation = 0x00;
    lithium_config.rx_freq = 437425;
    lithium_config.tx_freq = 437425;
    lithium_config.source[0] = 'N';
    lithium_config.source[1] = 'O';
    lithium_config.source[2] = 'C';
    lithium_config.source[3] = 'A';
    lithium_config.source[4] = 'L';
    lithium_config.source[5] = 'L';
    lithium_config.destination[0] = 'C';
    lithium_config.destination[1] = 'Q';
    lithium_config.destination[2] = 0x20;
    lithium_config.destination[3] = 0x20;
    lithium_config.destination[4] = 0x20;
    lithium_config.destination[5] = 0x20;
    lithium_config.tx_preamble = 0x10;
    lithium_config.tx_postamble = 0x00;
    lithium_config.function_config = 0x7001;
    lithium_config.function_config2 = 0x000;

    SetConfigurationCommand set_configuration_command(lithium_config);
    CHECK(Lithium::GetInstance()->DoCommand(&set_configuration_command));

    GetConfigurationCommand config_command;
    CHECK(Lithium::GetInstance()->DoCommand(&config_command));
    LithiumConfiguration received_configuration = config_command.GetParsedResponse();

    CHECK(lithium_config.interface_baud_rate ==
          received_configuration.interface_baud_rate);
    CHECK(lithium_config.tx_power_amp_level ==
          received_configuration.tx_power_amp_level);
    CHECK(lithium_config.rx_rf_baud_rate ==
          received_configuration.rx_rf_baud_rate);
    CHECK(lithium_config.tx_rf_baud_rate ==
          received_configuration.tx_rf_baud_rate);
    CHECK(lithium_config.rx_modulation == received_configuration.rx_modulation);
    CHECK(lithium_config.interface_baud_rate ==
          received_configuration.interface_baud_rate);
    CHECK(lithium_config.tx_modulation == received_configuration.tx_modulation);
    CHECK(lithium_config.rx_freq == received_configuration.rx_freq);
    CHECK(lithium_config.source[0] == received_configuration.source[0]);
    CHECK(lithium_config.source[1] == received_configuration.source[1]);
    CHECK(lithium_config.source[2] == received_configuration.source[2]);
    CHECK(lithium_config.source[3] == received_configuration.source[3]);
    CHECK(lithium_config.source[4] == received_configuration.source[4]);
    CHECK(lithium_config.source[5] == received_configuration.source[5]);
    CHECK(lithium_config.destination[0] ==
          received_configuration.destination[0]);
    CHECK(lithium_config.destination[1] ==
          received_configuration.destination[1]);
    CHECK(lithium_config.destination[2] ==
          received_configuration.destination[2]);
    CHECK(lithium_config.destination[3] ==
          received_configuration.destination[3]);
    CHECK(lithium_config.destination[4] ==
          received_configuration.destination[4]);
    CHECK(lithium_config.destination[5] ==
          received_configuration.destination[5]);
    CHECK(lithium_config.tx_preamble == received_configuration.tx_preamble);
    CHECK(lithium_config.tx_preamble == received_configuration.tx_preamble);
    CHECK(lithium_config.tx_postamble == received_configuration.tx_postamble);
    CHECK(lithium_config.function_config ==
          received_configuration.function_config);
    CHECK(lithium_config.function_config2 ==
          received_configuration.function_config2);
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
