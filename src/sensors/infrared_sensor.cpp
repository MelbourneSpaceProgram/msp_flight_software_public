#include <math.h>
#include <src/sensors/infrared_sensor.h>

const double p1 = -2.362272825932358e+03, p2 = 1.164697776531066e+04,
             p3 = -2.457026272978545e+04, p4 = 2.887425236691933e+04,
             p5 = -2.061119154675368e+04, p6 = 9.135824661573712e+03,
             p7 = -2.464410337515545e+03, p8 = 3.785871673586688e+02,
             p9 = -30.453780230032056, p10 = 2.973281695942479;

InfraredSensor::InfraredSensor(Matrix &side_normal_matrix)
    : side_normal(side_normal_matrix, data),
      infrared_reading(infrared_reading) {}

void InfraredSensor::InfraredToAngle() {
    double x = infrared_reading;
    angle_to_nadir = p1 * pow(x, 9) + p2 * pow(x, 8) + p3 * pow(x, 7) +
                     p4 * pow(x, 6) + p5 * pow(x, 5) + p6 * pow(x, 4) +
                     p7 * pow(x, 3) + p8 * pow(x, 2) + p9 * x + p10;
}

void InfraredSensor::SetInfraredReading(double value) {
    infrared_reading = value;
}

Matrix InfraredSensor::GetSideNormal() { return side_normal; }

double InfraredSensor::GetInfraredReading() { return infrared_reading; }

double InfraredSensor::GetAngleToNadir() {
    InfraredToAngle();
    return angle_to_nadir;
}
