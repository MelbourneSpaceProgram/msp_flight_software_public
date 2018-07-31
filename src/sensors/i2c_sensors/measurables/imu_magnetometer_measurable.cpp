#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>

const MagnetometerReading
    ImuMagnetometerMeasurable::kFailedMagnetometerReading = {-9999, -9999,
                                                             -9999, 0};

ImuMagnetometerMeasurable::ImuMagnetometerMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<MagnetometerReading>(imu_sensor,
                                         kFailedMagnetometerReading) {}

MagnetometerReading ImuMagnetometerMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    return imu_sensor->TakeMagnetometerReading();
}
