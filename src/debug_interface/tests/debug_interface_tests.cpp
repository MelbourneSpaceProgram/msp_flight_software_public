#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/config/unit_tests.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/SensorReading.pb.h>
#include <src/util/message_codes.h>
#include <test_runners/debug_interface_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

void TestRequestReceiveMessageFromSimulator() {
#ifdef RUN_HIL
    DebugStream *debug_stream = DebugStream::GetInstance();

    uint8_t buffer[SensorReading_size];

    // Entering critical section
    Semaphore_pend(debug_stream->bus_available, BIOS_WAIT_FOREVER);

    debug_stream->RequestMessageFromSimulator(kTestRequestCode);

    debug_stream->ReceiveMessageFromSimulator(buffer, SensorReading_size);
    Semaphore_post(debug_stream->bus_available);
    // Exited critical section

    pb_istream_t stream = pb_istream_from_buffer(buffer, SensorReading_size);

    SensorReading reading;
    pb_decode(&stream, SensorReading_fields, &reading);

    TEST_ASSERT_EQUAL_DOUBLE(1234, reading.value);
    TEST_ASSERT_EQUAL_INT(4321, reading.timestamp_millis_unix_epoch);
#else
    TEST_IGNORE_MESSAGE("HIL test ignored");
#endif
}

void TestPostMessageToDebugClient() {
#ifdef RUN_HIL
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
                         "__FILE__", __LINE__);
        throw e;
    }

    // Entering critical section
    Semaphore_pend(debug_stream->bus_available, BIOS_WAIT_FOREVER);
    debug_stream->PostMessageToDebugClient(kTestSensorReadingCode,
                                           SensorReading_size, buffer);
    debug_stream->ReceiveMessageFromSimulator(receive_buffer,
                                              SensorReading_size);
    Semaphore_post(debug_stream->bus_available);
    // Exited critical section

    pb_istream_t receive_stream =
        pb_istream_from_buffer(receive_buffer, SensorReading_size);

    SensorReading test_sensor_reading_received;
    pb_decode(&receive_stream, SensorReading_fields,
              &test_sensor_reading_received);

    TEST_ASSERT_EQUAL_DOUBLE(-999, test_sensor_reading_received.value);
    TEST_ASSERT_EQUAL_INT(
        123456789, test_sensor_reading_received.timestamp_millis_unix_epoch);
#else
    TEST_IGNORE_MESSAGE("HIL test ignored");
#endif
}
