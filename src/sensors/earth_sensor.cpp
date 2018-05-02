#include <math.h>
#include <src/sensors/earth_sensor.h>

EarthSensor::EarthSensor()
    : pos_x_side_normal(const_cast<double (&)[3][1]>(pos_x)),
      pos_y_side_normal(const_cast<double (&)[3][1]>(pos_y)),
      pos_z_side_normal(const_cast<double (&)[3][1]>(pos_z)),
      neg_x_side_normal(const_cast<double (&)[3][1]>(neg_x)),
      neg_y_side_normal(const_cast<double (&)[3][1]>(neg_x)),
      neg_z_side_normal(const_cast<double (&)[3][1]>(neg_x)),
      pos_x_sensor(pos_x_side_normal),
      pos_y_sensor(pos_y_side_normal),
      pos_z_sensor(pos_z_side_normal),
      neg_x_sensor(neg_x_side_normal),
      neg_y_sensor(neg_y_side_normal),
      neg_z_sensor(neg_z_side_normal),
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

    double pos_z_component_data[3][1];
    Matrix pos_z_component(pos_z_component_data);
    double neg_z_component_data[3][1];
    Matrix neg_z_component(neg_z_component_data);

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

    pos_z_component.MultiplyScalar(pos_z_sensor.GetSideNormal(),
                                   cos(pos_z_sensor.GetAngleToNadir()));
    neg_z_component.MultiplyScalar(pos_z_sensor.GetSideNormal(),
                                   cos(neg_z_sensor.GetAngleToNadir()));
    z_component.Subtract(pos_z_component, neg_z_component);
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

void EarthSensor::SetPosZSensorReading(double value) {
    pos_z_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegXSensorReading(double value) {
    neg_x_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegYSensorReading(double value) {
    neg_y_sensor.SetInfraredReading(value);
}

void EarthSensor::SetNegZSensorReading(double value) {
    neg_z_sensor.SetInfraredReading(value);
}

const Matrix EarthSensor::GetNadirVector() { return nadir_vector; }
