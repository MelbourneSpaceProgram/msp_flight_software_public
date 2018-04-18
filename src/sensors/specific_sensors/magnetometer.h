#ifndef SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
#define SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_

#include <src/database/circular_buffer_nanopb.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/generic_measurable.h>
#include <src/sensors/i2c_sensors/mpu9250_motion_tracker.h>
#include <string>

class Magnetometer : public GenericMeasurable<MagnetometerReading> {
   public:
    explicit Magnetometer();
    bool TakeReading();

   private:
    bool TakeReadingHil();

    CircularBufferNanopb<MagnetometerReading, MagnetometerReading_size,
                         MagnetometerReading_fields>
        calibration_readings_buffer_a;
    CircularBufferNanopb<MagnetometerReading, MagnetometerReading_size,
                         MagnetometerReading_fields>
        calibration_readings_buffer_b;
    MPU9250MotionTracker imu_a;
    MPU9250MotionTracker imu_b;

    static const uint8_t kImuAddress = 0x68;
    static const uint32_t kCalibrationReadingsBufferSize = 1000;
    static const char kCalibrationReadingsBufferAFileName[];
    static const char kCalibrationReadingsBufferBFileName[];
};

#endif  //  SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
