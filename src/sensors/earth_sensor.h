#ifndef SRC_SENSORS_EARTH_SENSOR_H_
#define SRC_SENSORS_EARTH_SENSOR_H_

#include <src/sensors/generic_measurable.h>
#include <src/sensors/infrared_sensor.h>
#include <src/util/matrix.h>

class EarthSensor {
   public:
    EarthSensor();
    void CalculateNadirVector();
    void SetPosXSensorReading(double value);
    void SetPosYSensorReading(double value);
    void SetNegXSensorReading(double value);
    void SetNegYSensorReading(double value);
    void SetNegZASensorReading(double value);
    void SetNegZBSensorReading(double value);
    Matrix GetNadirVector() const;

   private:
    static const double kPosX[3][1], kPosY[3][1], kNegX[3][1], kNegY[3][1],
        kNegZ[3][1];
    double nadir_vector_data[3][1];
    Matrix pos_x_side_normal;
    Matrix pos_y_side_normal;
    Matrix neg_x_side_normal;
    Matrix neg_y_side_normal;
    Matrix neg_z_side_normal;
    InfraredSensor pos_x_sensor;
    InfraredSensor pos_y_sensor;
    InfraredSensor neg_x_sensor;
    InfraredSensor neg_y_sensor;
    InfraredSensor neg_z_a_sensor;
    InfraredSensor neg_z_b_sensor;
    Matrix nadir_vector;
};

#endif /* SRC_SENSORS_EARTH_SENSOR_H_ */
