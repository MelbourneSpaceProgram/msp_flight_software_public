#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_GYROSCOPE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_GYROSCOPE_MEASURABLE_H_

#include <src/messages/GyroscopeReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

class ImuGyroscopeMeasurable : public I2cMeasurable<GyroscopeReading> {
   public:
    explicit ImuGyroscopeMeasurable(MPU9250MotionTracker* imu_sensor);
    GyroscopeReading TakeDirectI2cReading();
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_GYROSCOPE_MEASURABLE_H_
