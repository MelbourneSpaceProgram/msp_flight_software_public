#ifndef SRC_SENSORS_EARTH_SENSOR_H_
#define SRC_SENSORS_EARTH_SENSOR_H_

#include <src/sensors/infrared_sensor.h>
#include <src/util/matrix.h>
#include <src/messages/InfraredReading.pb.h>
#include <src/sensors/generic_measurable.h>

const double pos_x[3][1] = {{1}, {0}, {0}}, pos_y[3][1] = {{0}, {1}, {0}},
             pos_z[3][1] = {{0}, {0}, {1}}, neg_x[3][1] = {{-1}, {0}, {0}},
             neg_y[3][1] = {{0}, {-1}, {0}}, neg_z[3][1] = {{0}, {0}, {-1}};

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
    bool ReadSensors();

   private:
    bool ReadSensorsHil(InfraredReading &infrared_reading);
    InfraredReading infrared_reading_struct;
    double nadir_vector_data[3][1];
    Matrix pos_x_side_normal;
    Matrix pos_y_side_normal;
    Matrix neg_x_side_normal;
    Matrix neg_y_side_normal;
    Matrix neg_z_side_normal;
    Matrix nadir_vector;
    InfraredSensor pos_x_sensor;
    InfraredSensor pos_y_sensor;
    InfraredSensor neg_x_sensor;
    InfraredSensor neg_y_sensor;
    InfraredSensor neg_z_a_sensor;
    InfraredSensor neg_z_b_sensor;
};

#endif /* SRC_SENSORS_EARTH_SENSOR_H_ */
