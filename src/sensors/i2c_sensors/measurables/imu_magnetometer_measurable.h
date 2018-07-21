#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_MAGNETOMETER_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_MAGNETOMETER_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>

class ImuMagnetometerMeasurable : public I2cMeasurable<MagnetometerReading> {
public:
    explicit ImuMagnetometerMeasurable(MPU9250MotionTracker* imu_sensor);
    MagnetometerReading TakeI2cReading();
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_MAGNETOMETER_MEASURABLE_H_
