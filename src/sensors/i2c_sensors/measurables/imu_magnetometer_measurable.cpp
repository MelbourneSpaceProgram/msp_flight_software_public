#include <src/board/debug_interface/debug_stream.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/util/message_codes.h>

ImuMagnetometerMeasurable::ImuMagnetometerMeasurable(
    MPU9250MotionTracker* imu_sensor, const Matrix& frame_mapping,
    const Matrix& initial_biases, const Matrix& initial_scale_factors)
    : I2cMeasurable<MagnetometerReading>(imu_sensor,
                                         MagnetometerReading_init_default),
      magnetometer_to_body_frame_transform(frame_mapping),
      magnetometer_calibration(initial_biases, initial_scale_factors) {}

// Get readings from the hardware magnetometer and the simulation.
// Fuse the hardware and simulation readings for the controller, and
// echo readings to the DebugClient.
MagnetometerReading ImuMagnetometerMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    MagnetometerReading raw_reading = imu_sensor->TakeMagnetometerReading();

    // Apply change of frame from magnetometer frame to satellite body frame
    NewStackMatrixMacro(reading_magnetometer_frame, 3, 1);
    reading_magnetometer_frame.Set(0, 0, raw_reading.x);
    reading_magnetometer_frame.Set(1, 0, raw_reading.y);
    reading_magnetometer_frame.Set(2, 0, raw_reading.z);
    NewStackMatrixMacro(reading_body_frame, 3, 1);
    reading_body_frame.Multiply(magnetometer_to_body_frame_transform,
                                reading_magnetometer_frame);
    last_reading.x = reading_body_frame.Get(0, 0);
    last_reading.y = reading_body_frame.Get(1, 0);
    last_reading.z = reading_body_frame.Get(2, 0);

    if (kHilAvailable) {
        // Echo reading to DebugClient
        PostNanopbToSimMacro(MagnetometerReading, kMagnetometerReadingCode,
                             last_reading);
    }

    if (kHilAvailable) {
        MagnetometerReading simulation_reading = TakeSimulationReading();
        // Combine readings.
        // The static hardware reading will be calibrated out, and the
        // true hardware noise will be added to the simulated reading.
        last_reading.x = last_reading.x + simulation_reading.x;
        last_reading.y = last_reading.y + simulation_reading.y;
        last_reading.z = last_reading.z + simulation_reading.z;
    }

    magnetometer_calibration.Apply(last_reading);

    if (kHilAvailable) {
        PostNanopbToSimMacro(MagnetometerReading,
                             kCalibratedMagnetometerReadingCode, last_reading);
    }

    return last_reading;
}

MagnetometerReading ImuMagnetometerMeasurable::TakeSimulationReading() {
    uint8_t buffer[MagnetometerReading_size];
    MagnetometerReading simulation_reading = RequestNanopbFromSimMacro(
        MagnetometerReading, kMagnetometerReadingRequestCode);

    return simulation_reading;
}
