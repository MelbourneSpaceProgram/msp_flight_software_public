/*
 Adapted from  www.sailboatinstruments.blogspot.com
 */

#include <external/magnetometer_calibration_library/magnetometer_calibration_library.h>
#include <math.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/sensors/magnetometer_calibration.h>
#include <src/util/msp_exception.h>
#include <xdc/runtime/Log.h>

const char *MagnetometerCalibration::kBufferFilenameA = "magcalA.pb";
const char *MagnetometerCalibration::kBufferFilenameB = "magcalB.pb";

MagnetometerCalibration::MagnetometerCalibration(
    const Matrix &initial_biases, const Matrix &initial_scale_factors,
    const char *calibration_readings_buffer_filename)
    : biases(initial_biases, biases_data),
      scale_factors(initial_scale_factors, scale_factors_data),
      aggregated_readings(aggregated_readings_data),
      calibration_readings_buffer_filename(
          calibration_readings_buffer_filename) {
    if (kSdCardAvailable) {
        try {
            CircularBufferNanopb(MagnetometerReading)::Create(
                calibration_readings_buffer_filename,
                kCalibrationReadingsBufferSizeInReadings);
        } catch (etl::exception &e) {
            MspException::LogException(e);
            Log_error0("Circular Buffer/SD error");
        }
    }
}

void MagnetometerCalibration::AggregateReadings(const Matrix &mag_data) {
    NewStackMatrixMacro(D, 10, kBatchSizeInReadings);
    for (uint8_t i = 0; i < kBatchSizeInReadings; i++) {
        double x = mag_data.Get(i, 0);
        double y = mag_data.Get(i, 1);
        double z = mag_data.Get(i, 2);
        D.Set(0, i, x * x);
        D.Set(1, i, y * y);
        D.Set(2, i, z * z);
        D.Set(3, i, 2.0 * y * z);
        D.Set(4, i, 2.0 * x * z);
        D.Set(5, i, 2.0 * x * y);
        D.Set(6, i, 2.0 * x);
        D.Set(7, i, 2.0 * y);
        D.Set(8, i, 2.0 * z);
        D.Set(9, i, 1.0);
    }
    NewStackMatrixMacro(Dt, kBatchSizeInReadings, 10);
    Dt.Transpose(D);
    NewStackMatrixMacro(batch_aggregated_readings, 10, 10);
    batch_aggregated_readings.Multiply(D, Dt);
    // Matrix operations cannot be assumed to work in-place in general.
    // Add is ok.
    aggregated_readings.Add(aggregated_readings, batch_aggregated_readings);
}

void MagnetometerCalibration::GenerateScaleFactors(double *eigen_real3,
                                                   double hmb,
                                                   const Matrix &SSSS) {
    NewStackMatrixMacro(Dz, 3, 3);
    Dz.Fill(0);
    Dz.Set(0, 0, sqrt(eigen_real3[0]));
    Dz.Set(1, 1, sqrt(eigen_real3[1]));
    Dz.Set(2, 2, sqrt(eigen_real3[2]));

    NewStackMatrixMacro(vdz, 3, 3);
    vdz.Multiply(SSSS, Dz);

    NewStackMatrixMacro(SSSS_transpose, 3, 3);
    SSSS_transpose.Transpose(SSSS);

    NewStackMatrixMacro(SQ, 3, 3);
    SQ.Multiply(vdz, SSSS_transpose);

    scale_factors.MultiplyScalar(SQ, MagnetometerCalibrationLibrary::kHm / hmb);
}

void MagnetometerCalibration::GenerateBiases(const Matrix &v, const Matrix &Q) {
    NewStackMatrixMacro(U, 3, 1);

    U.Set(0, 0, v.Get(6, 0));
    U.Set(1, 0, v.Get(7, 0));
    U.Set(2, 0, v.Get(8, 0));

    NewStackMatrixMacro(Q_1, 3, 3);
    Q_1.InvertMatrix<3>(Q);

    // Calculate B = Q-1 * U   ( 3x1 = 3x3 * 3x1)
    biases.Multiply(Q_1, U);
    biases.Set(0, 0, -biases.Get(0, 0));
    biases.Set(1, 0, -biases.Get(1, 0));
    biases.Set(2, 0, -biases.Get(2, 0));
}

void MagnetometerCalibration::ComputeCalibrationParameters() {
    uint8_t i, j;
    NewStackMatrixMacro(S22a, 4, 6);
    NewStackMatrixMacro(v, 10, 1);
    NewStackMatrixMacro(Q, 3, 3);
    NewStackMatrixMacro(SS, 6, 6);

    MagnetometerCalibrationLibrary::ComputeMatrixSS(aggregated_readings, SS,
                                                    S22a);

    MagnetometerCalibrationLibrary::InitialiseEigenVector(SS, S22a, v);

    Q.Set(0, 0, v.Get(0, 0));
    Q.Set(0, 1, v.Get(5, 0));
    Q.Set(0, 2, v.Get(4, 0));
    Q.Set(1, 0, v.Get(5, 0));
    Q.Set(1, 1, v.Get(1, 0));
    Q.Set(1, 2, v.Get(3, 0));
    Q.Set(2, 0, v.Get(4, 0));
    Q.Set(2, 1, v.Get(3, 0));
    Q.Set(2, 2, v.Get(2, 0));

    double Q_array[9];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            Q_array[i * 3 + j] = Q.Get(i, j);
        }
    }

    GenerateBiases(v, Q);

    if (!kUsePreFlightMagnetometerCalibrationScaleFactors) {
        double hmb, norm1, norm2, norm3;
        NewStackMatrixMacro(SSSS, 3, 3);

        hmb = MagnetometerCalibrationLibrary::CalculateHMB(biases, Q, v);

        // Calculate SQ, the square root of matrix Q
        double SSSS_array[9];
        MagnetometerCalibrationLibrary::Hessenberg_Form_Elementary(
            Q_array, SSSS_array, 3);

        double eigen_real3[3];
        double eigen_imag3[3];
        MagnetometerCalibrationLibrary::QR_Hessenberg_Matrix(
            Q_array, SSSS_array, eigen_real3, eigen_imag3, 3, 100);

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                SSSS.Set(i, j, SSSS_array[i * 3 + j]);
            }
        }

        // normalize eigenvectors
        norm1 = sqrt(SSSS.Get(0, 0) * SSSS.Get(0, 0) +
                     SSSS.Get(1, 0) * SSSS.Get(1, 0) +
                     SSSS.Get(2, 0) * SSSS.Get(2, 0));
        SSSS.Set(0, 0, SSSS.Get(0, 0) / norm1);
        SSSS.Set(1, 0, SSSS.Get(1, 0) / norm1);
        SSSS.Set(2, 0, SSSS.Get(2, 0) / norm1);
        norm2 = sqrt(SSSS.Get(0, 1) * SSSS.Get(0, 1) +
                     SSSS.Get(1, 1) * SSSS.Get(1, 1) +
                     SSSS.Get(2, 1) * SSSS.Get(2, 1));
        SSSS.Set(0, 1, SSSS.Get(0, 1) / norm2);
        SSSS.Set(1, 1, SSSS.Get(1, 1) / norm2);
        SSSS.Set(2, 1, SSSS.Get(2, 1) / norm2);
        norm3 = sqrt(SSSS.Get(0, 2) * SSSS.Get(0, 2) +
                     SSSS.Get(1, 2) * SSSS.Get(1, 2) +
                     SSSS.Get(2, 2) * SSSS.Get(2, 2));
        SSSS.Set(0, 2, SSSS.Get(0, 2) / norm3);
        SSSS.Set(1, 2, SSSS.Get(1, 2) / norm3);
        SSSS.Set(2, 2, SSSS.Get(2, 2) / norm3);

        GenerateScaleFactors(eigen_real3, hmb, SSSS);
    }
}

Matrix MagnetometerCalibration::GetBiases() const { return biases; }

Matrix MagnetometerCalibration::GetScaleFactors() const {
    return scale_factors;
}

Matrix MagnetometerCalibration::GetAggregatedReadings() const {
    return aggregated_readings;
}

void MagnetometerCalibration::Apply(
    MagnetometerReading &magnetometer_reading_struct) {
    NewStackMatrixMacro(magnetometer_reading, 3, 1);
    magnetometer_reading.Set(0, 0, magnetometer_reading_struct.x);
    magnetometer_reading.Set(1, 0, magnetometer_reading_struct.y);
    magnetometer_reading.Set(2, 0, magnetometer_reading_struct.z);

    NewStackMatrixMacro(shifted_magnetometer_reading, 3, 1);
    shifted_magnetometer_reading.Subtract(magnetometer_reading, biases);

    NewStackMatrixMacro(shifted_scaled_magnetometer_reading, 3, 1);
    shifted_scaled_magnetometer_reading.Multiply(scale_factors,
                                                 shifted_magnetometer_reading);

    magnetometer_reading_struct.x =
        shifted_scaled_magnetometer_reading.Get(0, 0);
    magnetometer_reading_struct.y =
        shifted_scaled_magnetometer_reading.Get(1, 0);
    magnetometer_reading_struct.z =
        shifted_scaled_magnetometer_reading.Get(2, 0);
}

bool MagnetometerCalibration::Calibrate() {
    if (!kSdCardAvailable || kInstantDeploymentWaits) {
        return false;
    }

    // If the buffer doesn't have enough readings the
    // calibration could be invalid
    try {
        uint16_t n_messages_written =
            CircularBufferNanopb(MagnetometerReading)::ReadCountMessagesWritten(
                calibration_readings_buffer_filename);

        if (n_messages_written <
            MagnetometerCalibration::kMinimumSamplesForValidCalibration) {
            Log_warning0(
                "Not enough samples in the circular buffer to calibrate the "
                "magnetometer");
            return false;
        }
    } catch (etl::exception &e) {
        Log_error0("Could not read from circular buffer");
        return false;
    }

    for (uint32_t i = 0; i < kCalibrationReadingsBufferSizeInReadings /
                                 MagnetometerCalibration::kBatchSizeInReadings;
         i++) {
        // read data from circular buffer
        MagnetometerReading magnetometer_readings_batch
            [MagnetometerCalibration::kBatchSizeInReadings];
        for (uint8_t j = 0; j < MagnetometerCalibration::kBatchSizeInReadings;
             j++) {
            try {
                magnetometer_readings_batch[j] =
                    GetReadingFromBuffer(calibration_readings_buffer_filename);
            } catch (etl::exception &e) {
                Log_info0(
                    "Unable to read from magnetometer calibration circular "
                    "buffer");
                return false;
            }
        }

        double magnetometer_readings_batch_matrix_data
            [MagnetometerCalibration::kBatchSizeInReadings][3];
        Matrix magnetometer_readings_batch_matrix(
            magnetometer_readings_batch_matrix_data);
        for (uint16_t i = 0; i < MagnetometerCalibration::kBatchSizeInReadings;
             i++) {
            magnetometer_readings_batch_matrix.Set(
                i, 0, magnetometer_readings_batch[i].x);
            magnetometer_readings_batch_matrix.Set(
                i, 1, magnetometer_readings_batch[i].y);
            magnetometer_readings_batch_matrix.Set(
                i, 2, magnetometer_readings_batch[i].z);
        }
        AggregateReadings(magnetometer_readings_batch_matrix);
    }

    ComputeCalibrationParameters();

    return true;
}

MagnetometerReading MagnetometerCalibration::GetReadingFromBuffer(
    const char *file_name) {
    // Try to read from the buffer a number of times before giving up
    for (uint8_t i = 0; i < kBufferReadAttempts; i++) {
        try {
            return CircularBufferNanopb(MagnetometerReading)::ReadMessage(
                file_name);
        } catch (etl::exception &e) {
            // If the Hamming decoding fails, discard the
            // message and read another one.
            // Note: if too many readings fail, the calibration could
            // not have enough samples and be invalid.
            // This is considered unlikely.
            continue;
        }
    }
    etl::exception e("Unable to read valid data from buffer", __FILE__,
                     __LINE__);
    throw e;
}

bool MagnetometerCalibration::Store(MagnetometerReading magnetometer_reading) {
    try {
        // Write to circular buffer for calibration.
        // TODO (rskew) catch exceptions for pb encode error, sdcard write
        // error,
        CircularBufferNanopb(MagnetometerReading)::WriteMessage(
            calibration_readings_buffer_filename, magnetometer_reading);

        // Apply calibration operations
    } catch (etl::exception &e) {
        Log_error0("Can't write to circular buffer");
        return false;
    }
    return true;
}
