#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

ImuTemperatureMeasurable::ImuTemperatureMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<Mpu9250TemperatureReading>(imu_sensor) {}

Mpu9250TemperatureReading ImuTemperatureMeasurable::TakeI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    return imu_sensor->TakeTemperatureReading();
}
