#include <CppUTest/TestHarness.h>
#include <src/config/unit_tests.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/database/file_offsets.h>
#include <src/messages/MagnetometerReading.pb.h>

TEST_GROUP(CircularBuffer) {
    void setup() {
        if (!kEepromAvailable) {
            TEST_EXIT
        }
    };
};

// Overfill the buffer and read all data back to ensure circularity
TEST(CircularBuffer, WriteAndRead) {
    uint16_t file_offset = kFileOffsets[kMeasurableIdEnd];
    try {
        uint8_t n_messages = 4;
        CircularBufferNanopb(MagnetometerReading)::CreateIfNecessary(
            file_offset, n_messages);
        CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);
    } catch (MspException& e) {
        FAIL("Uncaught exception in test");
    }

    MagnetometerReading dummy_magnetometer_reading =
        MagnetometerReading_init_zero;
    dummy_magnetometer_reading.x = 1;
    dummy_magnetometer_reading.y = 2;
    dummy_magnetometer_reading.z = 3;
    dummy_magnetometer_reading.timestamp_ms = 123;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_offset, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);

    dummy_magnetometer_reading.x = 4;
    dummy_magnetometer_reading.y = 5;
    dummy_magnetometer_reading.z = 6;
    dummy_magnetometer_reading.timestamp_ms = 456;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_offset, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);

    dummy_magnetometer_reading.x = 7;
    dummy_magnetometer_reading.y = 8;
    dummy_magnetometer_reading.z = 9;
    dummy_magnetometer_reading.timestamp_ms = 789;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_offset, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);

    dummy_magnetometer_reading.x = 10;
    dummy_magnetometer_reading.y = 11;
    dummy_magnetometer_reading.z = 12;
    dummy_magnetometer_reading.timestamp_ms = 101112;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_offset, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);

    dummy_magnetometer_reading.x = 13;
    dummy_magnetometer_reading.y = 14;
    dummy_magnetometer_reading.z = 15;
    dummy_magnetometer_reading.timestamp_ms = 131415;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_offset, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_offset);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);
    DOUBLES_EQUAL(13, dummy_magnetometer_reading.x, 0.01);
    DOUBLES_EQUAL(14, dummy_magnetometer_reading.y, 0.01);
    DOUBLES_EQUAL(15, dummy_magnetometer_reading.z, 0.01);
    DOUBLES_EQUAL(131415, dummy_magnetometer_reading.timestamp_ms, 0.01);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_offset);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);
    DOUBLES_EQUAL(4, dummy_magnetometer_reading.x, 0.01);
    DOUBLES_EQUAL(5, dummy_magnetometer_reading.y, 0.01);
    DOUBLES_EQUAL(6, dummy_magnetometer_reading.z, 0.01);
    DOUBLES_EQUAL(456, dummy_magnetometer_reading.timestamp_ms, 0.01);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_offset);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);
    DOUBLES_EQUAL(7, dummy_magnetometer_reading.x, 0.01);
    DOUBLES_EQUAL(8, dummy_magnetometer_reading.y, 0.01);
    DOUBLES_EQUAL(9, dummy_magnetometer_reading.z, 0.01);
    DOUBLES_EQUAL(789, dummy_magnetometer_reading.timestamp_ms, 0.01);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_offset);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);
    DOUBLES_EQUAL(10, dummy_magnetometer_reading.x, 0.01);
    DOUBLES_EQUAL(11, dummy_magnetometer_reading.y, 0.01);
    DOUBLES_EQUAL(12, dummy_magnetometer_reading.z, 0.01);
    DOUBLES_EQUAL(101112, dummy_magnetometer_reading.timestamp_ms, 0.01);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_offset);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_offset);
    DOUBLES_EQUAL(13, dummy_magnetometer_reading.x, 0.01);
    DOUBLES_EQUAL(14, dummy_magnetometer_reading.y, 0.01);
    DOUBLES_EQUAL(15, dummy_magnetometer_reading.z, 0.01);
    DOUBLES_EQUAL(131415, dummy_magnetometer_reading.timestamp_ms, 0.01);
}
