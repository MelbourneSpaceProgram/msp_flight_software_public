#include <math.h>
#include <src/sensors/infrared_sensor.h>

const double sensor_a_poly_coeffs[10] = {
    -2.362272825932358e+03, 1.164697776531066e+04,  -2.457026272978545e+04,
    2.887425236691933e+04,  -2.061119154675368e+04, 9.135824661573712e+03,
    -2.464410337515545e+03, 3.785871673586688e+02,  -30.453780230032056,
    2.973281695942479};

InfraredSensor::InfraredSensor(Matrix &side_normal_matrix)
    : side_normal(side_normal_matrix, side_normal_data) {}

void InfraredSensor::InfraredToAngle() {
    double x = infrared_reading;
    angle_to_nadir = sensor_a_poly_coeffs[0] * pow(x, 9) +
                     sensor_a_poly_coeffs[1] * pow(x, 8) +
                     sensor_a_poly_coeffs[2] * pow(x, 7) +
                     sensor_a_poly_coeffs[3] * pow(x, 6) +
                     sensor_a_poly_coeffs[4] * pow(x, 5) +
                     sensor_a_poly_coeffs[5] * pow(x, 4) +
                     sensor_a_poly_coeffs[6] * pow(x, 3) +
                     sensor_a_poly_coeffs[7] * pow(x, 2) +
                     sensor_a_poly_coeffs[8] * x + sensor_a_poly_coeffs[9];
}

void InfraredSensor::SetInfraredReading(double value) {
    infrared_reading = value;
}

Matrix InfraredSensor::GetSideNormal() const { return side_normal; }

double InfraredSensor::GetInfraredReading() const { return infrared_reading; }

double InfraredSensor::GetAngleToNadir() {
    InfraredToAngle();
    return angle_to_nadir;
}
