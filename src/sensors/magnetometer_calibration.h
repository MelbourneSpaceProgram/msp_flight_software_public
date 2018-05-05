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
    void ComputeCalibrationParameters();
    void AggregateReadings(const Matrix &mag_data);
    Matrix GetBiases() const;
    Matrix GetScaleFactors() const;
    Matrix GetAggregatedReadings() const;
    void Apply(MagnetometerReading &magnetometer_reading_struct);

    static const uint8_t kBatchSizeInReadings = 10;

    // TODO (rskew) check this value
    static const uint32_t kMinimumSamplesForValidCalibration = 250;

   private:
    void GenerateBiases(const Matrix &v, const Matrix &Q);
    void GenerateScaleFactors(double *eigen_real3, double hmb,
                              const Matrix &SSSS);

    double aggregated_readings_data[10][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    double biases_data[3][1] = {{0}, {0}, {0}};
    ;
    double scale_factors_data[3][3] = {{0}, {0}, {0}};
    Matrix biases;
    Matrix scale_factors;
    Matrix aggregated_readings;
};

#endif /* SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_ */
