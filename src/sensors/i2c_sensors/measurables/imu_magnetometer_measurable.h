#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_MAGNETOMETER_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_MAGNETOMETER_MEASURABLE_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <src/sensors/magnetometer_calibration.h>
#include <src/util/matrix.h>

class ImuMagnetometerMeasurable : public I2cMeasurable<MagnetometerReading> {
   public:
    explicit ImuMagnetometerMeasurable(MPU9250MotionTracker *imu_sensor,
                                       const Matrix &frame_mapping,
                                       const Matrix &initial_biases,
                                       const Matrix &initial_scale_factors);
    MagnetometerReading TakeDirectI2cReading();
    MagnetometerReading TakeSimulationReading();
    bool Calibrate();

   private:
    const Matrix magnetometer_to_body_frame_transform;
    MagnetometerReading GetReadingFromBuffer(const char *file_name);
    MagnetometerCalibration magnetometer_calibration;

    // Reading at 1Hz for 2 hours requires a buffer size of
    // 60 * 60 * 2 = 7200 readings
    static const uint32_t kCalibrationReadingsBufferSizeInReadings = 7200;
    static const char *kCalibrationReadingsBufferFileName;
    // The number of sequential reads that have to fail before
    // we give up on calibration
    static const uint32_t kBufferReadAttempts = 500;

    static const MagnetometerReading kFailedMagnetometerReading;

    bool can_calibrate;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MEASURABLES_IMU_MAGNETOMETER_MEASURABLE_H_
