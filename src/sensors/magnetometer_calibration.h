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
                            const Matrix &initial_scale_factors,
                            const char *calibration_readings_buffer_filename);
    void ComputeCalibrationParameters();
    void AggregateReadings(const Matrix &mag_data);
    Matrix GetBiases() const;
    Matrix GetScaleFactors() const;
    Matrix GetAggregatedReadings() const;
    void Apply(MagnetometerReading &magnetometer_reading_struct);
    bool Calibrate();
    bool Store(MagnetometerReading magnetometer_reading);

    static MagnetometerReading GetReadingFromBuffer(const char *file_name);

    static const uint8_t kBatchSizeInReadings = 40;

    // TODO (rskew) check this value
    static const uint32_t kMinimumSamplesForValidCalibration = 250;

    static const char *MagnetometerCalibration::kBufferFilenameA;
    static const char *MagnetometerCalibration::kBufferFilenameB;

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
    double scale_factors_data[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Matrix biases;
    Matrix scale_factors;
    Matrix aggregated_readings;
    const char *calibration_readings_buffer_filename;

    // Reading at 1Hz for 2 hours requires a buffer size of
    // 60 * 60 * 2 = 7200 readings
    static const uint32_t kCalibrationReadingsBufferSizeInReadings = 7200;

    // The number of sequential reads that have to fail before
    // we give up on calibration
    static const uint32_t kBufferReadAttempts = 10;
};

#endif /* SRC_SENSORS_MAGNETOMETER_CALIBRATION_H_ */
