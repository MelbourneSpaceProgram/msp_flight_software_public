#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_TEMPERATURE_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

class ImuTemperatureMeasurable
    : public I2cMeasurable<Mpu9250TemperatureReading> {
   public:
    explicit ImuTemperatureMeasurable(MPU9250MotionTracker* imu_sensor);
    Mpu9250TemperatureReading TakeDirectI2cReading();

   private:
    static const Mpu9250TemperatureReading kFailedImuTemperatureReading;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_TEMPERATURE_MEASURABLE_H_
