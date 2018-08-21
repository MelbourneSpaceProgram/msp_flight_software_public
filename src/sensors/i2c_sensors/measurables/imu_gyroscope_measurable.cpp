#include <src/sensors/i2c_sensors/measurables/imu_gyroscope_measurable.h>

const GyroscopeReading ImuGyroscopeMeasurable::kFailedGyroscopeReading = {
    -9999, -9999, -9999, 0};

ImuGyroscopeMeasurable::ImuGyroscopeMeasurable(MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<GyroscopeReading>(imu_sensor, kFailedGyroscopeReading) {}

GyroscopeReading ImuGyroscopeMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    reading = imu_sensor->TakeGyroscopeReading();

    if (hil_available) {
        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kGyroscopeReadingCode, GyroscopeReading_size,
            GyroscopeReading_fields, &reading);
    }

    if (hil_available) {
        GyroscopeReading simulation_reading = TakeSimulationReading();
        // Combine readings.
        // The static hardware reading will be calibrated out, and the
        // true hardware noise will be added to the simulated reading.
        reading.x = reading.x + simulation_reading.x;
        reading.y = reading.y + simulation_reading.y;
        reading.z = reading.z + simulation_reading.z;
    }

    // TODO (rskew) write to circular buffer for calibration

    // TODO (rskew) apply calibration operations
}

GyroscopeReading ImuGyroscopeMeasurable::TakeSimulationReading() {
    DebugStream* debug_stream = DebugStream::GetInstance();
    uint8_t buffer[GyroscopeReading_size];
    GyroscopeReading simulation_reading;

    bool success = debug_stream->RequestMessageFromSimulator(
        kGyroscopeReadingRequestCode, buffer, GyroscopeReading_size);

    if (success) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, GyroscopeReading_size);

        bool status =
            pb_decode(&stream, GyroscopeReading_fields, &simulation_reading);
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
