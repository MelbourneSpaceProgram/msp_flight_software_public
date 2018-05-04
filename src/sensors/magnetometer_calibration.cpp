/*
 Adapted from  www.sailboatinstruments.blogspot.com
 */

#include <external/magnetometer_calibration_library/magnetometer_calibration_library.h>
#include <math.h>
#include <src/sensors/magnetometer_calibration.h>
#include <xdc/runtime/Log.h>

MagnetometerCalibration::MagnetometerCalibration() :
        biases(biases_data), scale_factors(scale_factors_data), aggregated_readings(
                aggregated_readings_data)
{
    biases.Fill(0);
    scale_factors.Identity();
    aggregated_readings.Fill(0);
}

void MagnetometerCalibration::AggregateReadings(const Matrix &mag_data)
{
    double D_data[10][kBatchSize];
    Matrix D(D_data);
    for (uint8_t i = 0; i < kBatchSize; i++)
    {
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
    double Dt_data[kBatchSize][10];
    Matrix Dt(Dt_data);
    Dt.Transpose(D);
    double batch_aggregated_readings_data[10][10];
    Matrix batch_aggregated_readings(batch_aggregated_readings_data);
    batch_aggregated_readings.Multiply(D, Dt);
    // Matrix operations cannot be assumed to work in-place in general.
    // Add is ok.
    aggregated_readings.Add(aggregated_readings, batch_aggregated_readings);
}

void MagnetometerCalibration::GenerateScaleFactors(double *eigen_real3,
                                                   double hmb,
                                                   const Matrix &SSSS)
{
    double Dz_data[3][3];
    Matrix Dz(Dz_data);
    Dz.Fill(0);
    Dz.Set(0, 0, sqrt(eigen_real3[0]));
    Dz.Set(1, 1, sqrt(eigen_real3[1]));
    Dz.Set(2, 2, sqrt(eigen_real3[2]));

    double vdz_data[3][3];
    Matrix vdz(vdz_data);
    vdz.Multiply(SSSS, Dz);

    double SSSS_transpose_data[3][3];
    Matrix SSSS_transpose(SSSS_transpose_data);
    SSSS_transpose.Transpose(SSSS);

    double SQ_data[3][3];
    Matrix SQ(SQ_data);
    SQ.Multiply(vdz, SSSS_transpose);

    scale_factors.MultiplyScalar(SQ, MagnetometerCalibrationLibrary::kHm / hmb);
}

void MagnetometerCalibration::GenerateBiases(const Matrix &v, const Matrix &Q)
{
    double U_data[3][1];
    Matrix U(U_data);

    U.Set(0, 0, v.Get(6, 0));
    U.Set(1, 0, v.Get(7, 0));
    U.Set(2, 0, v.Get(8, 0));

    double Q_data[3][3];
    Matrix Q_1(Q_data);
    Q_1.InvertMatrix<3>(Q);

    // Calculate B = Q-1 * U   ( 3x1 = 3x3 * 3x1)
    biases.Multiply(Q_1, U);
    biases.Set(0, 0, -biases.Get(0, 0));
    biases.Set(1, 0, -biases.Get(1, 0));
    biases.Set(2, 0, -biases.Get(2, 0));
}

void MagnetometerCalibration::ComputeCalibrationParameters()
{
    uint8_t i, j;
    double hmb, norm1, norm2, norm3;
    double S22a_data[4][6];
    double v_data[10][1];
    double Q_data[3][3];
    double SS_data[6][6];
    double SSSS_data[3][3];
    Matrix S22a(S22a_data);
    Matrix v(v_data);
    Matrix Q(Q_data);
    Matrix SS(SS_data);
    Matrix SSSS(SSSS_data);

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
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            Q_array[i * 3 + j] = Q.Get(i, j);
        }
    }

    GenerateBiases(v, Q);

    hmb = MagnetometerCalibrationLibrary::CalculateHMB(biases, Q, v);

    // Calculate SQ, the square root of matrix Q
    double SSSS_array[9];
    MagnetometerCalibrationLibrary::Hessenberg_Form_Elementary(Q_array,
                                                               SSSS_array, 3);

    double eigen_real3[3];
    double eigen_imag3[3];
    MagnetometerCalibrationLibrary::QR_Hessenberg_Matrix(Q_array, SSSS_array,
                                                         eigen_real3,
                                                         eigen_imag3, 3, 100);

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            SSSS.Set(i, j, SSSS_array[i * 3 + j]);
        }
    }

    // normalize eigenvectors
    norm1 = sqrt(
            SSSS.Get(0, 0) * SSSS.Get(0, 0) + SSSS.Get(1, 0) * SSSS.Get(1, 0)
                    + SSSS.Get(2, 0) * SSSS.Get(2, 0));
    SSSS.Set(0, 0, SSSS.Get(0, 0) / norm1);
    SSSS.Set(1, 0, SSSS.Get(1, 0) / norm1);
    SSSS.Set(2, 0, SSSS.Get(2, 0) / norm1);
    norm2 = sqrt(
            SSSS.Get(0, 1) * SSSS.Get(0, 1) + SSSS.Get(1, 1) * SSSS.Get(1, 1)
                    + SSSS.Get(2, 1) * SSSS.Get(2, 1));
    SSSS.Set(0, 1, SSSS.Get(0, 1) / norm2);
    SSSS.Set(1, 1, SSSS.Get(1, 1) / norm2);
    SSSS.Set(2, 1, SSSS.Get(2, 1) / norm2);
    norm3 = sqrt(
            SSSS.Get(0, 2) * SSSS.Get(0, 2) + SSSS.Get(1, 2) * SSSS.Get(1, 2)
                    + SSSS.Get(2, 2) * SSSS.Get(2, 2));
    SSSS.Set(0, 2, SSSS.Get(0, 2) / norm3);
    SSSS.Set(1, 2, SSSS.Get(1, 2) / norm3);
    SSSS.Set(2, 2, SSSS.Get(2, 2) / norm3);

    GenerateScaleFactors(eigen_real3, hmb, SSSS);

    Log_info3("Magnetometer Calibration Biases\n%d, %d, %d", biases.Get(0, 0),
            biases.Get(1, 0), biases.Get(2, 0));

    Log_info3("Magnetometer Calibration Scale Factors\n%d, %d, %d",
            scale_factors.Get(0, 0), scale_factors.Get(0, 1),
            scale_factors.Get(0, 2));
    Log_info3("%d, %d, %d", scale_factors.Get(1, 0), scale_factors.Get(1, 1),
            scale_factors.Get(1, 2));
    Log_info3("%d, %d, %d", scale_factors.Get(2, 0), scale_factors.Get(2, 1),
            scale_factors.Get(2, 2));
}

Matrix MagnetometerCalibration::GetBiases() const
{
    return biases;
}

Matrix MagnetometerCalibration::GetScaleFactors() const
{
    return scale_factors;
}

Matrix MagnetometerCalibration::GetAggregatedReadings() const
{
    return aggregated_readings;
}

void MagnetometerCalibration::Apply(
        MagnetometerReading &magnetometer_reading_struct)
{
    double magnetometer_reading_data[3][1];
    Matrix magnetometer_reading(magnetometer_reading_data);
    magnetometer_reading.Set(0, 0, magnetometer_reading_struct.x);
    magnetometer_reading.Set(1, 0, magnetometer_reading_struct.y);
    magnetometer_reading.Set(2, 0, magnetometer_reading_struct.z);

    double shifted_magnetometer_reading_data[3][1];
    Matrix shifted_magnetometer_reading(shifted_magnetometer_reading_data);
    shifted_magnetometer_reading.Subtract(magnetometer_reading, biases);

    double shifted_scaled_magnetometer_reading_data[3][1];
    Matrix shifted_scaled_magnetometer_reading(
            shifted_scaled_magnetometer_reading_data);
    shifted_scaled_magnetometer_reading.Multiply(scale_factors,
                                                 shifted_magnetometer_reading);

    magnetometer_reading_struct.x = shifted_scaled_magnetometer_reading.Get(0,
                                                                            0);
    magnetometer_reading_struct.y = shifted_scaled_magnetometer_reading.Get(1,
                                                                            0);
    magnetometer_reading_struct.z = shifted_scaled_magnetometer_reading.Get(2,
                                                                            0);
}
