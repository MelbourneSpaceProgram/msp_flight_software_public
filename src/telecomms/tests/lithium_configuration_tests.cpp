#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_configuration.h>
#include <src/telecomms/lithium_utils.h>

uint32_t FourBytesToUint32(byte byte1, byte byte2, byte byte3, byte byte4) {
    return static_cast<uint32_t>((byte1 << 24) | (byte2 << 16) | (byte3 << 8) |
                                 byte4);
}

TEST_GROUP(LithiumConfiguration){};

TEST(LithiumConfiguration, TestLithiumConfigurationDeserialise) {
    GetConfigurationCommand command;
    byte* command_buffer = command.GetReplyBuffer();

    // TODO(wschuet): Use SetConfiguration to create a serialised configuration
    command_buffer[0] = 0x00;
    command_buffer[1] = 0x01;
    command_buffer[2] = 0x02;
    command_buffer[3] = 0x03;
    command_buffer[4] = 0x04;
    command_buffer[5] = 0x05;
    command_buffer[6] = 0x06;
    command_buffer[7] = 0x07;
    command_buffer[8] = 0x08;
    command_buffer[9] = 0x09;
    command_buffer[10] = 0x0a;
    command_buffer[11] = 0x0b;
    command_buffer[12] = 0x0c;
    command_buffer[13] = 0x0d;
    command_buffer[14] = 0x0e;
    command_buffer[15] = 0x10;
    command_buffer[16] = 0x11;
    command_buffer[17] = 0x12;
    command_buffer[18] = 0x13;
    command_buffer[19] = 0x14;
    command_buffer[20] = 0x15;
    command_buffer[21] = 0x16;
    command_buffer[22] = 0x17;
    command_buffer[23] = 0x18;
    command_buffer[24] = 0x19;
    command_buffer[25] = 0x1a;
    command_buffer[26] = 0x1b;
    command_buffer[27] = 0x1c;
    command_buffer[28] = 0x1d;
    command_buffer[29] = 0x1e;
    command_buffer[30] = 0x20;
    command_buffer[31] = 0x21;
    command_buffer[32] = 0x22;
    command_buffer[33] = 0x23;

    LithiumConfig received_configuration = command.GetParsedResponse();

    CHECK_EQUAL(received_configuration.interface_baud_rate,
                static_cast<uint8_t>(command_buffer[0]));
    CHECK_EQUAL(received_configuration.tx_power_amp_level,
                static_cast<uint8_t>(command_buffer[1]));
    CHECK_EQUAL(received_configuration.rx_rf_baud_rate,
                static_cast<uint8_t>(command_buffer[2]));
    CHECK_EQUAL(received_configuration.tx_rf_baud_rate,
                static_cast<uint8_t>(command_buffer[3]));
    CHECK_EQUAL(received_configuration.rx_modulation,
                static_cast<uint8_t>(command_buffer[4]));
    CHECK_EQUAL(received_configuration.tx_modulation,
                static_cast<uint8_t>(command_buffer[5]));
    CHECK_EQUAL(received_configuration.rx_freq,
                FourBytesToUint32(command_buffer[9], command_buffer[8],
                                  command_buffer[7], command_buffer[6]));
    CHECK_EQUAL(received_configuration.tx_freq,
                FourBytesToUint32(command_buffer[13], command_buffer[12],
                                  command_buffer[11], command_buffer[10]));
    CHECK_EQUAL(received_configuration.source[0], command_buffer[14]);
    CHECK_EQUAL(received_configuration.source[1], command_buffer[15]);
    CHECK_EQUAL(received_configuration.source[2], command_buffer[16]);
    CHECK_EQUAL(received_configuration.source[3], command_buffer[17]);
    CHECK_EQUAL(received_configuration.source[4], command_buffer[18]);
    CHECK_EQUAL(received_configuration.source[5], command_buffer[19]);
    CHECK_EQUAL(received_configuration.destination[0], command_buffer[20]);
    CHECK_EQUAL(received_configuration.destination[1], command_buffer[21]);
    CHECK_EQUAL(received_configuration.destination[2], command_buffer[22]);
    CHECK_EQUAL(received_configuration.destination[3], command_buffer[23]);
    CHECK_EQUAL(received_configuration.destination[4], command_buffer[24]);
    CHECK_EQUAL(received_configuration.destination[5], command_buffer[25]);

    CHECK_EQUAL(
        received_configuration.tx_preamble,
        static_cast<uint16_t>(command_buffer[27] << 8 | command_buffer[26]));
    CHECK_EQUAL(
        received_configuration.tx_postamble,
        static_cast<uint16_t>(command_buffer[29] << 8 | command_buffer[28]));
    CHECK_EQUAL(
        received_configuration.function_config,
        static_cast<uint16_t>(command_buffer[31] << 8 | command_buffer[30]));
    CHECK_EQUAL(
        received_configuration.function_config2,
        static_cast<uint16_t>(command_buffer[33] << 8 | command_buffer[32]));
}
