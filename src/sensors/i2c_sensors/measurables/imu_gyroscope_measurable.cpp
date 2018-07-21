#include <src/sensors/i2c_sensors/measurables/imu_gyroscope_measurable.h>

ImuGyroscopeMeasurable::ImuGyroscopeMeasurable(MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<GyroscopeReading>(imu_sensor) {}

GyroscopeReading ImuGyroscopeMeasurable::TakeI2cReading() {
    MPU9250MotionTracker* imu_sensor = static_cast<MPU9250MotionTracker*> (I2cMeasurable::sensor);
    return imu_sensor->TakeGyroscopeReading();
}
