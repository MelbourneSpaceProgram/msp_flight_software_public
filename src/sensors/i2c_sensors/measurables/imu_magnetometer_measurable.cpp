#include <src/board/debug_interface/debug_stream.h>
#include <src/config/satellite.h>
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

    // Apply calibration operations
    magnetometer_calibration.Apply(last_reading);

    return last_reading;
}
