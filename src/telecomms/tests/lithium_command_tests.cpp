#include <src/telecomms/lithium.h>
#include <src/util/data_types.h>
#include <test_runners/lithium_command_tests.h>
#include <test_runners/unity.h>

void TestNoOpSerialisation() {
    NoOpCommand test_command;
    byte command_buffer[8];
    SerialisedMessage serial_command = test_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    TEST_ASSERT_EQUAL_UINT16(8, serial_command.GetSize());
    TEST_ASSERT_EQUAL_UINT16(8, test_command.GetSerialisedSize());
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('H'), serial_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('e'), serial_buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(0x10, serial_buffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0x01, serial_buffer[3]);
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[4]);
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[5]);

    // Checksum bits
    TEST_ASSERT_EQUAL_UINT8(serial_buffer[6], 0x11);
    TEST_ASSERT_EQUAL_UINT8(serial_buffer[7], 0x43);
}

void TestResetSystemSerialisation() {
    ResetSystemCommand reset_command;
    byte command_buffer[8];
    SerialisedMessage serial_command =
        reset_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    TEST_ASSERT_EQUAL_UINT16(8, serial_command.GetSize());
    TEST_ASSERT_EQUAL_UINT16(8, reset_command.GetSerialisedSize());
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('H'), serial_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('e'), serial_buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(0x10, serial_buffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0x02, serial_buffer[3]);
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[4]);
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[5]);

    // Checksum bits
    TEST_ASSERT_EQUAL_UINT8(serial_buffer[6], 0x12);
    TEST_ASSERT_EQUAL_UINT8(serial_buffer[7], 0x46);
}

void TestGetConfigurationSerialisation() {
    GetConfigurationCommand config_command;
    byte command_buffer[8];
    SerialisedMessage serial_command =
        config_command.SerialiseTo(command_buffer);
    byte* serial_buffer = serial_command.GetBuffer();

    TEST_ASSERT_EQUAL_UINT16(8, serial_command.GetSize());
    TEST_ASSERT_EQUAL_UINT16(8, config_command.GetSerialisedSize());
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('H'), serial_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('e'), serial_buffer[1]);
    TEST_ASSERT_EQUAL_UINT8(0x10, serial_buffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0x05, serial_buffer[3]);
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[4]);
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[5]);

    // Checksum bits
    TEST_ASSERT_EQUAL_UINT8(serial_buffer[6], 0x15);
    TEST_ASSERT_EQUAL_UINT8(serial_buffer[7], 0x4f);
}

void TestTransmitTestPayload() {
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
    TEST_ASSERT_EQUAL_UINT16(8 + 4 + 56 + 2, serial_command.GetSize());
    TEST_ASSERT_EQUAL_UINT16(8 + 4 + 56 + 2,
                             transmit_command.GetSerialisedSize());
    // Sync chars
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('H'), serial_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>('e'), serial_buffer[1]);
    // Command direction
    TEST_ASSERT_EQUAL_UINT8(0x10, serial_buffer[2]);
    // Command code
    TEST_ASSERT_EQUAL_UINT8(0x03, serial_buffer[3]);
    // Payload size bit 1
    TEST_ASSERT_EQUAL_UINT8(0x00, serial_buffer[4]);
    // Payload size: MSP Header + Payload Size = 4 + 57 = 0x3C
    TEST_ASSERT_EQUAL_UINT8(0x3C, serial_buffer[5]);
    // Header checksum
    TEST_ASSERT_EQUAL_UINT8(0x4f, serial_buffer[6]);
    TEST_ASSERT_EQUAL_UINT8(0x85, serial_buffer[7]);
    // TX count
    TEST_ASSERT_EQUAL_UINT8(0x67, serial_buffer[8]);
    // Total RX count
    TEST_ASSERT_EQUAL_UINT8(0x61, serial_buffer[9]);
    // Valid RX count
    TEST_ASSERT_EQUAL_UINT8(0x62, serial_buffer[10]);
    // Payload code
    TEST_ASSERT_EQUAL_UINT8(0x69, serial_buffer[11]);
    // Payload: +8 is for the Lithium header, +4 is for the MSP header
    TEST_ASSERT_EQUAL_UINT8_ARRAY(test_message, serial_buffer + 8 + 4, 55);
    // Check the carriage return
    TEST_ASSERT_EQUAL_UINT8(0x0d, serial_buffer[67]);
    // Tail checksum
    TEST_ASSERT_EQUAL_UINT8(0xbf, serial_buffer[68]);
    TEST_ASSERT_EQUAL_UINT8(0xb0, serial_buffer[69]);
}
