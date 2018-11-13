/*
  Adapted from  www.sailboatinstruments.blogspot.com
*/

#ifndef SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_
#define SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_

#include <src/messages/MagnetometerReading.pb.h>
#include <src/util/matrix.h>

class MagnetometerCalibration {
   public:
    MagnetometerCalibration(const Matrix &initial_biases,
                            const Matrix &initial_scale_factors);
    Matrix GetBiases() const;
    Matrix GetScaleFactors() const;
    void Apply(MagnetometerReading &magnetometer_reading_struct);

   private:
    double biases_data[3][1] = {{0}, {0}, {0}};
    double scale_factors_data[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Matrix biases;
    Matrix scale_factors;
};

#endif /* SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_ */
