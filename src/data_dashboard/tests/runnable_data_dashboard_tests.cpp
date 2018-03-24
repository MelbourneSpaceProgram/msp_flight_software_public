#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/SensorReading.pb.h>
#include <src/util/data_types.h>
#include <src/util/message_codes.h>
#include <test_runners/runnable_data_dashboard_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <src/config/unit_tests.h>

void TestTransmitMessage() {
    if (!hil_enabled) {
        TEST_IGNORE_MESSAGE("HIL test ignored");
    }
    DebugStream* debug_stream = DebugStream::GetInstance();
    byte buffer[SensorReading_size];

    SensorReading test_sensor_reading_message;
    test_sensor_reading_message.value = 987654321;
    test_sensor_reading_message.timestamp_millis_unix_epoch = 554455445;

    // Entering critical section
    Semaphore_pend(debug_stream->bus_available, BIOS_WAIT_FOREVER);

    RunnableDataDashboard::TransmitMessage(
        kTestSensorReadingCode, SensorReading_size, SensorReading_fields,
        &test_sensor_reading_message);

    debug_stream->ReceiveMessageFromSimulator(buffer, SensorReading_size);

    Semaphore_post(debug_stream->bus_available);
    // Exited critical section

    pb_istream_t stream = pb_istream_from_buffer(buffer, SensorReading_size);

    SensorReading reading;
    pb_decode(&stream, SensorReading_fields, &reading);

    TEST_ASSERT_EQUAL_DOUBLE(987654321, reading.value);
    TEST_ASSERT_EQUAL_INT(554455445, reading.timestamp_millis_unix_epoch);
}
