/*
  Adapted from  www.sailboatinstruments.blogspot.com
*/

#ifndef SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_
#define SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_

#include <src/util/matrix.h>

class MagnetometerCalibration {
   public:
    MagnetometerCalibration();
    void ComputeCalibrationParameters();
    void ComputeAggregatedReadings(const Matrix &mag_data);
    Matrix GetBiases() const;
    Matrix GetScaleFactors() const;
    Matrix GetAggregatedReadings() const;

    static const uint8_t kDataSize = 40;

   private:
    void GenerateBiases(const Matrix &v, const Matrix &Q);
    void GenerateScaleFactors(double *eigen_real3, double hmb,
                              const Matrix &SSSS);

    double aggregated_readings_data[10][10];
    double biases_data[3][1];
    double scale_factors_data[3][3];
    Matrix biases;
    Matrix scale_factors;
    Matrix aggregated_readings;
};

#endif /* SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_ */
