#include <CppUTest/TestHarness.h>
#include <src/telecomms/lithium.h>
#include <src/telecomms/lithium_commands/fast_pa_command.h>
#include <src/telecomms/lithium_commands/get_configuration_command.h>
#include <src/telecomms/lithium_commands/no_op_command.h>
#include <src/telecomms/lithium_commands/reset_system_command.h>
#include <src/telecomms/lithium_commands/transmit_command.h>
#include <src/telecomms/lithium_commands/write_flash_command.h>
#include <src/telecomms/lithium_md5.h>
#include <src/telecomms/lithium_utils.h>
#include <src/telecomms/msp_payloads/test_payload.h>
#include <src/util/data_types.h>

TEST_GROUP(LithiumCommandSerialise){};

TEST(LithiumCommandSerialise, TestNoOpSerialisation) {
    NoOpCommand test_command;
    byte command_buffer[8];
    SerialisedMessage serial_command = test_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    CHECK_EQUAL(8, serial_command.GetSize());
    CHECK_EQUAL(8, test_command.GetSerialisedSize());
    CHECK_EQUAL(static_cast<uint8_t>('H'), serial_buffer[0]);
    CHECK_EQUAL(static_cast<uint8_t>('e'), serial_buffer[1]);
    CHECK_EQUAL(0x10, serial_buffer[2]);
    CHECK_EQUAL(0x01, serial_buffer[3]);
    CHECK_EQUAL(0x00, serial_buffer[4]);
    CHECK_EQUAL(0x00, serial_buffer[5]);

    // Checksum bits
    CHECK_EQUAL(serial_buffer[6], 0x11);
    CHECK_EQUAL(serial_buffer[7], 0x43);
}

TEST(LithiumCommandSerialise, TestResetSystemSerialisation) {
    ResetSystemCommand reset_command;
    byte command_buffer[8];
    SerialisedMessage serial_command =
        reset_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    CHECK_EQUAL(8, serial_command.GetSize());
    CHECK_EQUAL(8, reset_command.GetSerialisedSize());
    CHECK_EQUAL(static_cast<uint8_t>('H'), serial_buffer[0]);
    CHECK_EQUAL(static_cast<uint8_t>('e'), serial_buffer[1]);
    CHECK_EQUAL(0x10, serial_buffer[2]);
    CHECK_EQUAL(0x02, serial_buffer[3]);
    CHECK_EQUAL(0x00, serial_buffer[4]);
    CHECK_EQUAL(0x00, serial_buffer[5]);

    // Checksum bits
    CHECK_EQUAL(serial_buffer[6], 0x12);
    CHECK_EQUAL(serial_buffer[7], 0x46);
}

TEST(LithiumCommandSerialise, TestGetConfigurationSerialisation) {
    GetConfigurationCommand config_command;
    byte command_buffer[8];
    SerialisedMessage serial_command =
        config_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    CHECK_EQUAL(8, serial_command.GetSize());
    CHECK_EQUAL(8, config_command.GetSerialisedSize());
    CHECK_EQUAL(static_cast<uint8_t>('H'), serial_buffer[0]);
    CHECK_EQUAL(static_cast<uint8_t>('e'), serial_buffer[1]);
    CHECK_EQUAL(0x10, serial_buffer[2]);
    CHECK_EQUAL(0x05, serial_buffer[3]);
    CHECK_EQUAL(0x00, serial_buffer[4]);
    CHECK_EQUAL(0x00, serial_buffer[5]);

    // Checksum bits
    CHECK_EQUAL(serial_buffer[6], 0x15);
    CHECK_EQUAL(serial_buffer[7], 0x4f);
}

TEST(LithiumCommandSerialise, TestTransmitTestPayloadSerialisation) {
    TestPayload test_payload;
    TransmitCommand transmit_command(&test_payload, 0x67, 0x61, 0x62);
    // 8 + 4 + 56 + 2 is Lithium header + MSP header + Message size + Tail
    byte command_buffer[8 + 4 + 56 + 2];
    SerialisedMessage serial_command =
        transmit_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();
    const char* test_message =
        "This is a test message from the Melbourne Space Program";

    // Size
    CHECK_EQUAL(8 + 4 + 56 + 2, serial_command.GetSize());
    CHECK_EQUAL(8 + 4 + 56 + 2, transmit_command.GetSerialisedSize());
    // Sync chars
    CHECK_EQUAL(static_cast<uint8_t>('H'), serial_buffer[0]);
    CHECK_EQUAL(static_cast<uint8_t>('e'), serial_buffer[1]);
    // Command direction
    CHECK_EQUAL(0x10, serial_buffer[2]);
    // Command code
    CHECK_EQUAL(0x03, serial_buffer[3]);
    // Payload size bit 1
    CHECK_EQUAL(0x00, serial_buffer[4]);
    // Payload size: MSP Header + Payload Size = 4 + 57 = 0x3C
    CHECK_EQUAL(0x3C, serial_buffer[5]);
    // Header checksum
    CHECK_EQUAL(0x4f, serial_buffer[6]);
    CHECK_EQUAL(0x85, serial_buffer[7]);
    // TX count
    CHECK_EQUAL(0x67, serial_buffer[8]);
    // Total RX count
    CHECK_EQUAL(0x61, serial_buffer[9]);
    // Valid RX count
    CHECK_EQUAL(0x62, serial_buffer[10]);
    // Payload code
    CHECK_EQUAL(0x69, serial_buffer[11]);
    // Payload: +8 is for the Lithium header, +4 is for the MSP header
    MEMCMP_EQUAL(test_message, serial_buffer + 8 + 4, 55);
    // Check the carriage return
    CHECK_EQUAL(0x0d, serial_buffer[67]);
    // Tail checksum
    CHECK_EQUAL(0xbf, serial_buffer[68]);
    CHECK_EQUAL(0xb0, serial_buffer[69]);
}

TEST(LithiumCommandSerialise, TestWriteFlashSerialisation) {
    byte command_buffer[16 + 8 + 2];

    etl::array<byte, LithiumMd5::kNumMd5Bytes> md5_bytes = {
        0x9b, 0x20, 0x4f, 0xc6, 0x5f, 0x0f, 0x1e, 0x60,
        0x7f, 0xc1, 0x82, 0x89, 0x6d, 0x81, 0xc1, 0x12};

    // TODO(dingbenjamin): Figure out a way to use unity array assert with etl
    // array
    byte equivalent_md5_bytes[LithiumMd5::kNumMd5Bytes] = {
        0x9b, 0x20, 0x4f, 0xc6, 0x5f, 0x0f, 0x1e, 0x60,
        0x7f, 0xc1, 0x82, 0x89, 0x6d, 0x81, 0xc1, 0x12};

    LithiumMd5 md5_message(&md5_bytes);
    WriteFlashCommand flash_command(&md5_message);

    SerialisedMessage serial_command =
        flash_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    // Size
    CHECK_EQUAL(8 + 16 + 2, serial_command.GetSize());
    CHECK_EQUAL(8 + 16 + 2, flash_command.GetSerialisedSize());
    // Sync chars
    CHECK_EQUAL(static_cast<uint8_t>('H'), serial_buffer[0]);
    CHECK_EQUAL(static_cast<uint8_t>('e'), serial_buffer[1]);
    // Command direction
    CHECK_EQUAL(0x10, serial_buffer[2]);
    // Command code
    CHECK_EQUAL(0x08, serial_buffer[3]);
    // Payload size
    CHECK_EQUAL(0x00, serial_buffer[4]);
    CHECK_EQUAL(0x10, serial_buffer[5]);
    // Header checksum
    CHECK_EQUAL(0x28, serial_buffer[6]);
    CHECK_EQUAL(0x68, serial_buffer[7]);
    // Payload
    MEMCMP_EQUAL(equivalent_md5_bytes, serial_buffer + 8, 16);
    // Tail checksum
    CHECK_EQUAL(0x80, serial_buffer[8 + 16]);
    CHECK_EQUAL(0x48, serial_buffer[8 + 16 + 1]);
}

TEST(LithiumCommandSerialise, TestFastPaSerialisation) {
    // Header + Payload + Tail
    byte serial_buffer[8 + 1 + 2];
    uint8_t test_pa_level = 92;
    FastPaCommand test_pa_command(test_pa_level);

    SerialisedMessage serial_command =
        test_pa_command.SerialiseTo(serial_buffer);

    // Size
    CHECK_EQUAL(8 + 1 + 2, serial_command.GetSize());
    CHECK_EQUAL(8 + 1 + 2, test_pa_command.GetSerialisedSize());
    // Sync chars
    CHECK_EQUAL(static_cast<uint8_t>('H'), serial_buffer[0]);
    CHECK_EQUAL(static_cast<uint8_t>('e'), serial_buffer[1]);
    // Command direction
    CHECK_EQUAL(0x10, serial_buffer[2]);
    // Command code
    CHECK_EQUAL(0x20, serial_buffer[3]);
    // Payload size
    CHECK_EQUAL(0x00, serial_buffer[4]);
    CHECK_EQUAL(0x01, serial_buffer[5]);
    // TODO(dingbenjamin): Header checksum
    // Payload
    CHECK_EQUAL(92, serial_buffer[8]);
    // TODO(dingbenjamin): Tail checksum
}
