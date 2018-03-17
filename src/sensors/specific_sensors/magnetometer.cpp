#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/generic_sensor.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

Magnetometer::Magnetometer(std::string id) : id(id) {}

bool Magnetometer::TakeReading() {
#ifdef RUN_HIL
    return TakeReadingHil();
#endif

    // TODO(rskew): Implement the I2C read
    return false;
}

bool Magnetometer::TakeReadingHil() {
    DebugStream *debug_stream = DebugStream::GetInstance();
    uint8_t buffer[MagnetometerReading_size];

    // Entering critical section
    Semaphore_pend(debug_stream->bus_available, BIOS_WAIT_FOREVER);
    debug_stream->RequestMessageFromSimulator(kMagnetometerReadingRequestCode);
    debug_stream->ReceiveMessageFromSimulator(buffer, MagnetometerReading_size);
    Semaphore_post(debug_stream->bus_available);
    // Exited critical section

    pb_istream_t stream =
        pb_istream_from_buffer(buffer, MagnetometerReading_size);

    bool status = pb_decode(&stream, MagnetometerReading_fields, &reading);
    if (!status) {
        etl::exception e("Magnetometer::TakeReadingHil pb_decode failed",
                         "__FILE__", __LINE__);
        throw e;
    }

    NotifyObservers();

    // TODO(rskew) put this code in the controller task
    // (done in bdot branch)
    // ==========================================================
    RunnableDataDashboard::TransmitMessage(
        kMagnetometerReadingCode, MagnetometerReading_size,
        MagnetometerReading_fields, &reading);

    TorqueOutputReading torque_output_reading = TorqueOutputReading_init_zero;
    torque_output_reading.x = 0;
    torque_output_reading.y = 0;
    torque_output_reading.z = 0.05;

    RunnableDataDashboard::TransmitMessage(
        kTorqueOutputReadingCode, TorqueOutputReading_size,
        TorqueOutputReading_fields, &torque_output_reading);
    // ==========================================================

    return true;
}
