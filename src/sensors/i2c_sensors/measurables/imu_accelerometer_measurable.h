#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_ACCELEROMETER_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_ACCELEROMETER_MEASURABLE_H_

#include <src/messages/AccelerometerReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

class ImuAccelerometerMeasurable : public I2cMeasurable<AccelerometerReading> {
   public:
    explicit ImuAccelerometerMeasurable(MPU9250MotionTracker* imu_sensor);
    AccelerometerReading TakeDirectI2cReading();
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_ACCELEROMETER_MEASURABLE_H_
