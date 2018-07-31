#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

ImuTemperatureMeasurable::ImuTemperatureMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<double>(imu_sensor, kFailedImuTemperatureReading) {}

double ImuTemperatureMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    return imu_sensor->TakeTemperatureReading();
}
