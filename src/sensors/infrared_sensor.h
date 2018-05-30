#ifndef SRC_SENSORS_INFRARED_SENSOR_H_
#define SRC_SENSORS_INFRARED_SENSOR_H_

#include <src/util/matrix.h>

class InfraredSensor {
   public:
    explicit InfraredSensor(Matrix &side_normal_matrix);
    Matrix GetSideNormal() const;
    double GetInfraredReading() const;
    double GetAngleToNadir();
    void SetInfraredReading(double value);

   private:
    void InfraredToAngle();
    static const double kSensorAPolyCoeffs[10];
    double side_normal_data[3][1];
    Matrix side_normal;
    double infrared_reading;
    double angle_to_nadir;
};

#endif /* SRC_SENSORS_INFRARED_SENSOR_H_ */
