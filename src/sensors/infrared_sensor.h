#ifndef INFRARED_SENSOR_H_
#define INFRARED_SENSOR_H_

#include <src/util/matrix.h>

class InfraredSensor {
   public:
    InfraredSensor(Matrix &side_normal_matrix);
    Matrix GetSideNormal();
    double GetInfraredReading();
    double GetAngleToNadir();
    void SetInfraredReading(double value);

   private:
    void InfraredToAngle();

    double data[3][1];
    Matrix side_normal;
    double infrared_reading;
    double angle_to_nadir;
};

#endif /* INFRARED_SENSOR_H_ */
