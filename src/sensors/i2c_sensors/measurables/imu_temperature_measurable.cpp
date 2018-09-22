#include <src/sensors/i2c_sensors/measurables/imu_temperature_measurable.h>

ImuTemperatureMeasurable::ImuTemperatureMeasurable(
    MPU9250MotionTracker* imu_sensor)
    : I2cMeasurable<TemperatureReading>(imu_sensor,
                                        TemperatureReading_init_default) {}

TemperatureReading ImuTemperatureMeasurable::TakeDirectI2cReading() {
    MPU9250MotionTracker* imu_sensor =
        static_cast<MPU9250MotionTracker*>(I2cMeasurable::sensor);
    TemperatureReading reading = TemperatureReading_init_default;
    reading.temp = imu_sensor->TakeTemperatureReading();
    return reading;
}
