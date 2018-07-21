#include <src/sensors/i2c_sensors/measurables/imu_accelerometer_measurable.h>

ImuAccelerometerMeasurable::ImuAccelerometerMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<AccelerometerReading>(imu_sensor) {}

AccelerometerReading ImuAccelerometerMeasurable::TakeI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    return imu_sensor->TakeAccelerometerReading();
}
