#ifndef SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
#define SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_

#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/generic_measurable.h>

class Magnetometer : public GenericMeasurable<MagnetometerReading> {
   public:
    explicit Magnetometer()
        // :
        // circular_buffer_calibration_data(calibration_readings_buffer_size_in_messages,
        //                                    calibration_readings_buffer_file_name)
        {};
    bool TakeReading();

   private:
    bool TakeReadingHil();
    // CircularBufferNanopb<MagnetometerReading,
    //                      MagnetometerReading_size,
    //                      MagnetometerReading_fields>
    //                      calibration_readings_buffer;
    // static const uint32_t calibration_readings_buffer_size = 1000;
    // static const char* calibration_readings_buffer_file_name =
    //     "magnetometer_calibration_readings";

    // static const char* kMagnetometerCalibrationDataFilename =
    //   "magnetometer_calibration_data";
};

#endif  //  SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
