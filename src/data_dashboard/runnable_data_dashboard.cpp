#include <external/etl/exception.h>
#include <external/nanopb/pb_encode.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/messages/SensorReading.pb.h>
#include <src/system/state_definitions.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <string>

RunnableDataDashboard::RunnableDataDashboard() {}

fnptr RunnableDataDashboard::GetRunnablePointer() {
    return &RunnableDataDashboard::DataDashboard;
}

void RunnableDataDashboard::DataDashboard() {
    // TODO(rskew) remove this when actual sensor readings are implemented
    double bms1_current = 0.0;
    double increment = 0.01;

    while (1) {
        // TODO(rskew) replace this with stubs of real sensors
        SensorReading test_sensor_reading_message;
        test_sensor_reading_message.value = 12345;
        test_sensor_reading_message.timestamp_millis_unix_epoch = 54321;

        RunnableDataDashboard::TransmitMessage(
            kTestSensorReadingCode, SensorReading_size, SensorReading_fields,
            &test_sensor_reading_message);

        Task_sleep(50);
    }
}

void RunnableDataDashboard::TransmitMessage(const byte message_code,
                                            const uint8_t message_size,
                                            const pb_field_t* message_fields,
                                            const void* message_struct) {
    DebugStream* debug_stream = DebugStream::GetInstance();
    uint8_t buffer[256];
    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    status = pb_encode(&stream, message_fields, message_struct);

    if (!status) {
        etl::exception e(
            "RunnableDataDashboard::TransmitMessage pb_encode failed",
            "__FILE__", __LINE__);
        throw e;
    }

    debug_stream->PostMessageToDebugClient(message_code, message_size, buffer);
}
