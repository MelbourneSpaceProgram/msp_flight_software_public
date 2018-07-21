#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>

ImuMagnetometerMeasurable::ImuMagnetometerMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<MagnetometerReading>(imu_sensor) {}

MagnetometerReading ImuMagnetometerMeasurable::TakeI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    return imu_sensor->TakeMagnetometerReading();
}
