#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <test_runners/circular_buffer_nanopb_tests.h>

// Overfill the buffer and read all data back to ensure circularity
void TestWriteAndRead() {
    char* file_name = "crclbuf.tst";
    uint8_t n_messages = 4;
    CircularBufferNanopb(MagnetometerReading)::Create(file_name, n_messages);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);

    MagnetometerReading dummy_magnetometer_reading =
        MagnetometerReading_init_zero;
    dummy_magnetometer_reading.x = 1;
    dummy_magnetometer_reading.y = 2;
    dummy_magnetometer_reading.z = 3;
    dummy_magnetometer_reading.timestamp_millis_unix_epoch = 123;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_name, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);

    dummy_magnetometer_reading.x = 4;
    dummy_magnetometer_reading.y = 5;
    dummy_magnetometer_reading.z = 6;
    dummy_magnetometer_reading.timestamp_millis_unix_epoch = 456;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_name, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);

    dummy_magnetometer_reading.x = 7;
    dummy_magnetometer_reading.y = 8;
    dummy_magnetometer_reading.z = 9;
    dummy_magnetometer_reading.timestamp_millis_unix_epoch = 789;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_name, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);

    dummy_magnetometer_reading.x = 10;
    dummy_magnetometer_reading.y = 11;
    dummy_magnetometer_reading.z = 12;
    dummy_magnetometer_reading.timestamp_millis_unix_epoch = 101112;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_name, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);

    dummy_magnetometer_reading.x = 13;
    dummy_magnetometer_reading.y = 14;
    dummy_magnetometer_reading.z = 15;
    dummy_magnetometer_reading.timestamp_millis_unix_epoch = 131415;
    CircularBufferNanopb(MagnetometerReading)::WriteMessage(
        file_name, dummy_magnetometer_reading);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_name);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);
    TEST_ASSERT_EQUAL_DOUBLE(13, dummy_magnetometer_reading.x);
    TEST_ASSERT_EQUAL_DOUBLE(14, dummy_magnetometer_reading.y);
    TEST_ASSERT_EQUAL_DOUBLE(15, dummy_magnetometer_reading.z);
    TEST_ASSERT_EQUAL_DOUBLE(
        131415, dummy_magnetometer_reading.timestamp_millis_unix_epoch);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_name);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);
    TEST_ASSERT_EQUAL_DOUBLE(4, dummy_magnetometer_reading.x);
    TEST_ASSERT_EQUAL_DOUBLE(5, dummy_magnetometer_reading.y);
    TEST_ASSERT_EQUAL_DOUBLE(6, dummy_magnetometer_reading.z);
    TEST_ASSERT_EQUAL_DOUBLE(
        456, dummy_magnetometer_reading.timestamp_millis_unix_epoch);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_name);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);
    TEST_ASSERT_EQUAL_DOUBLE(7, dummy_magnetometer_reading.x);
    TEST_ASSERT_EQUAL_DOUBLE(8, dummy_magnetometer_reading.y);
    TEST_ASSERT_EQUAL_DOUBLE(9, dummy_magnetometer_reading.z);
    TEST_ASSERT_EQUAL_DOUBLE(
        789, dummy_magnetometer_reading.timestamp_millis_unix_epoch);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_name);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);
    TEST_ASSERT_EQUAL_DOUBLE(10, dummy_magnetometer_reading.x);
    TEST_ASSERT_EQUAL_DOUBLE(11, dummy_magnetometer_reading.y);
    TEST_ASSERT_EQUAL_DOUBLE(12, dummy_magnetometer_reading.z);
    TEST_ASSERT_EQUAL_DOUBLE(
        101112, dummy_magnetometer_reading.timestamp_millis_unix_epoch);

    dummy_magnetometer_reading =
        CircularBufferNanopb(MagnetometerReading)::ReadMessage(file_name);
    CircularBufferNanopb(MagnetometerReading)::ReadHeader(file_name);
    TEST_ASSERT_EQUAL_DOUBLE(13, dummy_magnetometer_reading.x);
    TEST_ASSERT_EQUAL_DOUBLE(14, dummy_magnetometer_reading.y);
    TEST_ASSERT_EQUAL_DOUBLE(15, dummy_magnetometer_reading.z);
    TEST_ASSERT_EQUAL_DOUBLE(
        131415, dummy_magnetometer_reading.timestamp_millis_unix_epoch);
}
