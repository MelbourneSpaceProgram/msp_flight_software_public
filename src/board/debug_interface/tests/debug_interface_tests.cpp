#include <CppUTest/TestHarness.h>
#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/messages/SensorReading.pb.h>
#include <src/util/message_codes.h>

TEST_GROUP(DebugInterface) {
    void setup() {
        if (!kHilAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(DebugInterface, TestRequestMessageFromSimulator) {
    DebugStream *debug_stream = DebugStream::GetInstance();

    uint8_t buffer[SensorReading_size];

    bool success = debug_stream->RequestMessageFromSimulator(
        kTestRequestCode, buffer, SensorReading_size);
    if (!success) {
        FAIL("Debug stream could not perform request");
    }

    pb_istream_t stream = pb_istream_from_buffer(buffer, SensorReading_size);

    SensorReading reading;
    pb_decode(&stream, SensorReading_fields, &reading);

    DOUBLES_EQUAL(1234, reading.value, 1.0);
    CHECK_EQUAL(4321, reading.timestamp_ms);
}

TEST(DebugInterface, TestPostMessageToDebugClient) {
    DebugStream *debug_stream = DebugStream::GetInstance();

    SensorReading test_sensor_reading;
    test_sensor_reading.value = -999;
    test_sensor_reading.timestamp_ms = 123456789;

    uint8_t buffer[SensorReading_size];
    uint8_t receive_buffer[SensorReading_size];

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status;
    status = pb_encode(&stream, SensorReading_fields, &test_sensor_reading);
    if (!status) {
        FAIL("TestPostMessageToDebugClient pb_encode failed");
    }

    debug_stream->PostMessageToDebugClient(kTestSensorReadingCode,
                                           SensorReading_size, buffer);
    bool success = debug_stream->RequestMessageFromSimulator(
        kTestSensorReadingRequestCode, receive_buffer, SensorReading_size);
    if (!success) {
        FAIL("Debug stream could not perform request");
    }

    pb_istream_t receive_stream =
        pb_istream_from_buffer(receive_buffer, SensorReading_size);

    SensorReading test_sensor_reading_received;
    pb_decode(&receive_stream, SensorReading_fields,
              &test_sensor_reading_received);

    DOUBLES_EQUAL(-999, test_sensor_reading_received.value, 1);
    CHECK_EQUAL(123456789,
                test_sensor_reading_received.timestamp_ms);

    // Reset the test value on the DebugClient end.
    // Stateful tests are no fun.
    SensorReading reset_test_sensor_reading;
    reset_test_sensor_reading.value = 0;
    reset_test_sensor_reading.timestamp_ms = 0;

    uint8_t reset_buffer[SensorReading_size];
    pb_ostream_t reset_stream =
        pb_ostream_from_buffer(reset_buffer, sizeof(reset_buffer));

    bool reset_status;
    reset_status = pb_encode(&reset_stream, SensorReading_fields,
                             &reset_test_sensor_reading);
    if (!reset_status) {
        FAIL("TestPostMessageToDebugClient pb_encode failed");
    }
    debug_stream->PostMessageToDebugClient(kTestSensorReadingCode,
                                           SensorReading_size, reset_buffer);
}
