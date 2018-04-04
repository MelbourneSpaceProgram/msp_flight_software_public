#include <external/etl/exception.h>
#include <external/nanopb/pb_encode.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/init/init.h>
#include <src/messages/SensorReading.pb.h>
#include <src/messages/StateMachineStateReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/system/state_definitions.h>
#include <src/system/system_state_machines/adcs_state_machine.h>
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

    //while (1) {
        SensorReading bms1_input_current_reading = SensorReading_init_zero;
        // TODO(rskew) implement actual sensor readings
        bms1_current += increment;
        bms1_input_current_reading.value = bms1_current;

        TransmitMessage(kBms1InputCurrentReadingCode, SensorReading_size,
                        SensorReading_fields, &bms1_input_current_reading);

        SensorReading bms1_input_voltage_reading = SensorReading_init_zero;
        bms1_input_voltage_reading.value = 88.88;

        TransmitMessage(kBms1InputVoltageReadingCode, SensorReading_size,
                        SensorReading_fields, &bms1_input_voltage_reading);

        SensorReading primary_mcu_regulator_current_reading =
            SensorReading_init_zero;
        primary_mcu_regulator_current_reading.value = 77.77;

        TransmitMessage(kPrimaryMcuRegulatorCurrentReadingCode,
                        SensorReading_size, SensorReading_fields,
                        &primary_mcu_regulator_current_reading);

        SensorReading magnetorquer_x_current_reading = SensorReading_init_zero;
        magnetorquer_x_current_reading.value = 66.66;

        TransmitMessage(kMagnetorquerXCurrentReadingCode, SensorReading_size,
                        SensorReading_fields, &magnetorquer_x_current_reading);

        StateMachineStateReading adcs_system_state_reading =
            StateMachineStateReading_init_zero;
        adcs_system_state_reading.state = (uint32_t)kAdcsNominal;

        TransmitMessage(
            kAdcsSystemStateReadingCode, StateMachineStateReading_size,
            StateMachineStateReading_fields, &adcs_system_state_reading);

    //    Task_sleep(50);
    //}
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
