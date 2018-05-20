#include <math.h>
#include <src/sensors/infrared_sensor.h>

InfraredSensor::InfraredSensor(Matrix &side_normal_matrix)
    : side_normal(side_normal_matrix, side_normal_data),
      infrared_reading(infrared_reading),
      angle_to_nadir(angle_to_nadir) {}

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
