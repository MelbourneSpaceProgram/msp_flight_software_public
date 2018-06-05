#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/messages/SensorReading.pb.h>
#include <src/util/message_codes.h>
#include <test_runners/unity.h>

void TestRequestMessageFromSimulator() {
    if (!hil_enabled) {
        TEST_IGNORE_MESSAGE("HIL test ignored");
    }
    DebugStream *debug_stream = DebugStream::GetInstance();

    uint8_t buffer[SensorReading_size];

    bool success = debug_stream->RequestMessageFromSimulator(
        kTestRequestCode, buffer, SensorReading_size);
    if (!success) {
        TEST_IGNORE_MESSAGE("Debug stream could not perform request");
    }

    pb_istream_t stream = pb_istream_from_buffer(buffer, SensorReading_size);

    SensorReading reading;
    pb_decode(&stream, SensorReading_fields, &reading);

    TEST_ASSERT_EQUAL_DOUBLE(1234, reading.value);
    TEST_ASSERT_EQUAL_INT(4321, reading.timestamp_millis_unix_epoch);
}

void TestPostMessageToDebugClient() {
    if (!hil_enabled) {
        TEST_IGNORE_MESSAGE("HIL test ignored");
    }
    DebugStream *debug_stream = DebugStream::GetInstance();

    SensorReading test_sensor_reading;
    test_sensor_reading.value = -999;
    test_sensor_reading.timestamp_millis_unix_epoch = 123456789;

    uint8_t buffer[SensorReading_size];
    uint8_t receive_buffer[SensorReading_size];

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status;
    status = pb_encode(&stream, SensorReading_fields, &test_sensor_reading);
    if (!status) {
        etl::exception e("TestPostMessageToDebugClient pb_encode failed",
                         __FILE__, __LINE__);
        throw e;
    }

    debug_stream->PostMessageToDebugClient(kTestSensorReadingCode,
                                           SensorReading_size, buffer);
    bool success = debug_stream->RequestMessageFromSimulator(
        kTestSensorReadingRequestCode, receive_buffer, SensorReading_size);
    if (!success) {
        TEST_IGNORE_MESSAGE("Debug stream could not perform request");
    }

    pb_istream_t receive_stream =
        pb_istream_from_buffer(receive_buffer, SensorReading_size);

    SensorReading test_sensor_reading_received;
    pb_decode(&receive_stream, SensorReading_fields,
              &test_sensor_reading_received);

    TEST_ASSERT_EQUAL_DOUBLE(-999, test_sensor_reading_received.value);
    TEST_ASSERT_EQUAL_INT(
        123456789, test_sensor_reading_received.timestamp_millis_unix_epoch);

    // Reset the test value on the DebugClient end.
    // Stateful tests are no fun.
    SensorReading reset_test_sensor_reading;
    reset_test_sensor_reading.value = 0;
    reset_test_sensor_reading.timestamp_millis_unix_epoch = 0;

    uint8_t reset_buffer[SensorReading_size];
    pb_ostream_t reset_stream =
        pb_ostream_from_buffer(reset_buffer, sizeof(reset_buffer));

    bool reset_status;
    reset_status = pb_encode(&reset_stream, SensorReading_fields,
                             &reset_test_sensor_reading);
    if (!reset_status) {
        etl::exception e("TestPostMessageToDebugClient pb_encode failed",
                         __FILE__, __LINE__);
        throw e;
    }
    debug_stream->PostMessageToDebugClient(kTestSensorReadingCode,
                                           SensorReading_size, reset_buffer);
}
