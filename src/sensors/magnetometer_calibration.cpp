#include <math.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/sensors/magnetometer_calibration.h>
#include <src/util/msp_exception.h>
#include <xdc/runtime/Log.h>

MagnetometerCalibration::MagnetometerCalibration(
    const Matrix &initial_biases, const Matrix &initial_scale_factors)
    : biases(initial_biases, biases_data),
      scale_factors(initial_scale_factors, scale_factors_data) {}

Matrix MagnetometerCalibration::GetBiases() const { return biases; }

Matrix MagnetometerCalibration::GetScaleFactors() const {
    return scale_factors;
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
