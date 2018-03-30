#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/sensors/generic_measurable.h>
#include <src/sensors/specific_sensors/magnetometer.h>
#include <src/util/message_codes.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>

Magnetometer::Magnetometer(std::string id) : id(id) {}

bool Magnetometer::TakeReading() {
    if (hil_enabled) {
        return TakeReadingHil();
    }

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
    // (done in HIL_ADCS branch)
    // ==========================================================

    // Echo reading to data dashboard
    RunnableDataDashboard::TransmitMessage(
        kMagnetometerReadingCode, MagnetometerReading_size,
        MagnetometerReading_fields, &reading);

    // Run estimator
    BDotEstimator b_dot_estimator(50, 400);

    double magnetometer_reading_data[3][1] = {
        {reading.x}, {reading.y}, {reading.z}};
    Matrix magnetometer_reading(magnetometer_reading_data);
    double b_dot_estimate_data[3][1];
    Matrix b_dot_estimate(b_dot_estimate_data);

    b_dot_estimator.Estimate(magnetometer_reading, b_dot_estimate);

    // TODO(rskew) tell DetumbledStateMachine about Bdot (or omega?)

    // Run controller
    double torque_output_data[3][1];
    Matrix torque_output(torque_output_data);

    BDotController::Control(magnetometer_reading, b_dot_estimate,
                            torque_output);

    // Send torque output to simulation
    TorqueOutputReading torque_output_reading = TorqueOutputReading_init_zero;
    torque_output_reading.x = torque_output.Get(0, 0);
    torque_output_reading.y = torque_output.Get(1, 0);
    torque_output_reading.z = torque_output.Get(2, 0);

    RunnableDataDashboard::TransmitMessage(
        kTorqueOutputReadingCode, TorqueOutputReading_size,
        TorqueOutputReading_fields, &torque_output_reading);
    // ==========================================================

    return true;
}
