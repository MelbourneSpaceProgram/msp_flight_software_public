#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_TEMPERATURE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_TEMPERATURE_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

class ImuTemperatureMeasurable : public I2cMeasurable<double> {
   public:
    explicit ImuTemperatureMeasurable(MPU9250MotionTracker* imu_sensor);
    double TakeDirectI2cReading();

   private:
    static const double kFailedImuTemperatureReading = -9999;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_TEMPERATURE_MEASURABLE_H_
