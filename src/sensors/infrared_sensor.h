#ifndef SRC_SENSORS_INFRARED_SENSOR_H_
#define SRC_SENSORS_INFRARED_SENSOR_H_

#include <src/util/matrix.h>

const double sensor_a_poly_coeffs[10] =
        { -2.362272825932358e+03, 1.164697776531066e+04, -2.457026272978545e+04,
          2.887425236691933e+04, -2.061119154675368e+04, 9.135824661573712e+03,
          -2.464410337515545e+03, 3.785871673586688e+02, -30.453780230032056,
          2.973281695942479 };

class InfraredSensor {
public:
    explicit InfraredSensor(Matrix &side_normal_matrix);
    Matrix GetSideNormal() const;
    double GetInfraredReading() const;
    double GetAngleToNadir();
    void SetInfraredReading(double value);

private:
    void InfraredToAngle();

    double side_normal_data[3][1];
    Matrix side_normal;
    double infrared_reading;
    double angle_to_nadir;
};

#endif /* SRC_SENSORS_INFRARED_SENSOR_H_ */
