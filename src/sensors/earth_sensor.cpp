#include <math.h>
#include <src/sensors/earth_sensor.h>

// Defined surface normal (unit) vectors in all six sides of the satellite
const double EarthSensor::kPosX[3][1] = {{1}, {0}, {0}},
             EarthSensor::kPosY[3][1] = {{0}, {1}, {0}},
             EarthSensor::kNegX[3][1] = {{-1}, {0}, {0}},
             EarthSensor::kNegY[3][1] = {{0}, {-1}, {0}},
             EarthSensor::kNegZ[3][1] = {{0}, {0}, {-1}};

EarthSensor::EarthSensor()
    : pos_x_side_normal(const_cast<double (&)[3][1]>(kPosX)),
      pos_y_side_normal(const_cast<double (&)[3][1]>(kPosY)),
      neg_x_side_normal(const_cast<double (&)[3][1]>(kNegX)),
      neg_y_side_normal(const_cast<double (&)[3][1]>(kNegY)),
      neg_z_side_normal(const_cast<double (&)[3][1]>(kNegZ)),
      pos_x_sensor(pos_x_side_normal),
      pos_y_sensor(pos_y_side_normal),
      neg_x_sensor(neg_x_side_normal),
      neg_y_sensor(neg_y_side_normal),
      neg_z_a_sensor(neg_z_side_normal),
      neg_z_b_sensor(neg_z_side_normal),
      nadir_vector(nadir_vector_data) {}

void EarthSensor::CalculateNadirVector() {
    double x_component_data[3][1];
    Matrix x_component(x_component_data);
    double y_component_data[3][1];
    Matrix y_component(y_component_data);
    double z_component_data[3][1];
    Matrix z_component(z_component_data);

    double pos_x_component_data[3][1];
    Matrix pos_x_component(pos_x_component_data);
    double neg_x_component_data[3][1];
    Matrix neg_x_component(neg_x_component_data);

    double pos_y_component_data[3][1];
    Matrix pos_y_component(pos_y_component_data);
    double neg_y_component_data[3][1];
    Matrix neg_y_component(neg_y_component_data);

    double neg_z_a_component_data[3][1];
    Matrix neg_z_a_component(neg_z_a_component_data);
    double neg_z_b_component_data[3][1];
    Matrix neg_z_b_component(neg_z_b_component_data);

    pos_x_component.MultiplyScalar(pos_x_sensor.GetSideNormal(),
                                   cos(pos_x_sensor.GetAngleToNadir()));
    neg_x_component.MultiplyScalar(pos_x_sensor.GetSideNormal(),
                                   cos(neg_x_sensor.GetAngleToNadir()));
    x_component.Subtract(pos_x_component, neg_x_component);
    x_component.MultiplyScalar(x_component, 0.5);

    pos_y_component.MultiplyScalar(pos_y_sensor.GetSideNormal(),
                                   cos(pos_y_sensor.GetAngleToNadir()));
    neg_y_component.MultiplyScalar(pos_y_sensor.GetSideNormal(),
                                   cos(neg_y_sensor.GetAngleToNadir()));
    y_component.Subtract(pos_y_component, neg_y_component);
    y_component.MultiplyScalar(y_component, 0.5);

    neg_z_a_component.MultiplyScalar(neg_z_a_sensor.GetSideNormal(),
                                     cos(neg_z_a_sensor.GetAngleToNadir()));
    neg_z_b_component.MultiplyScalar(neg_z_b_sensor.GetSideNormal(),
                                     cos(neg_z_b_sensor.GetAngleToNadir()));
    z_component.Add(neg_z_a_component, neg_z_b_component);
    z_component.MultiplyScalar(z_component, 0.5);

    nadir_vector.Add(x_component, y_component);
    nadir_vector.Add(nadir_vector, z_component);
}

void EarthSensor::SetPosXSensorReading(double value) {
    pos_x_sensor.SetInfraredReading(value);
}

void EarthSensor::SetPosYSensorReading(double value) {
    pos_y_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegXSensorReading(double value) {
    neg_x_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegYSensorReading(double value) {
    neg_y_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegZASensorReading(double value) {
    neg_z_a_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegZBSensorReading(double value) {
    neg_z_b_sensor.SetInfraredReading(value);
}

Matrix EarthSensor::GetNadirVector() const { return nadir_vector; }
