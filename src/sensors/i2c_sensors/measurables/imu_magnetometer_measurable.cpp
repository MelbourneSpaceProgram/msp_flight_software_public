#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/util/message_codes.h>

const MagnetometerReading
    ImuMagnetometerMeasurable::kFailedMagnetometerReading = {-9999, -9999,
                                                             -9999, 0};

ImuMagnetometerMeasurable::ImuMagnetometerMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<MagnetometerReading>(imu_sensor,
                                         kFailedMagnetometerReading) {}

// Get readings from the hardware magnetometer and the simulation.
// Fuse the hardware and simulation readings for the controller, and
// echo readings to the DebugClient.
MagnetometerReading ImuMagnetometerMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    reading = imu_sensor->TakeMagnetometerReading();

    if (hil_available) {
        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kMagnetometerReadingCode, MagnetometerReading_size,
            MagnetometerReading_fields, &reading);
    }

    if (hil_available) {
        MagnetometerReading simulation_reading = TakeSimulationReading();
        // Combine readings.
        // The static hardware reading will be calibrated out, and the
        // true hardware noise will be added to the simulated reading.
        reading.x = reading.x + simulation_reading.x;
        reading.y = reading.y + simulation_reading.y;
        reading.z = reading.z + simulation_reading.z;
    }

    // TODO (rskew) write to circular buffer for calibration

    // TODO (rskew) apply calibration operations

    return reading;
}

MagnetometerReading ImuMagnetometerMeasurable::TakeSimulationReading() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    uint8_t buffer[MagnetometerReading_size];
    MagnetometerReading simulation_reading;

    bool success = debug_stream->RequestMessageFromSimulator(
        kMagnetometerReadingRequestCode, buffer, MagnetometerReading_size);

    if (success) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, MagnetometerReading_size);

        bool status =
            pb_decode(&stream, MagnetometerReading_fields, &simulation_reading);
        if (!status) {
            etl::exception e("pb_decode failed", __FILE__, __LINE__);
            throw e;
        }
    } else {
        etl::exception e("Failed to request message from simulator", __FILE__,
                         __LINE__);
        throw e;
    }

    return simulation_reading;
}
