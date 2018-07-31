#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

const Mpu9250TemperatureReading
    ImuTemperatureMeasurable::kFailedImuTemperatureReading = {-9999};

ImuTemperatureMeasurable::ImuTemperatureMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<Mpu9250TemperatureReading>(imu_sensor,
                                               kFailedImuTemperatureReading) {}

Mpu9250TemperatureReading ImuTemperatureMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    return imu_sensor->TakeTemperatureReading();
}
