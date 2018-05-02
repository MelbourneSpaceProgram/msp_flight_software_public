#include <external/nanopb/pb_decode.h>
#include <math.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/debug_interface/debug_stream.h>
#include <src/sensors/earth_sensor.h>
#include <src/util/message_codes.h>
#include <xdc/runtime/Log.h>

EarthSensor::EarthSensor()
    : pos_x_side_normal(const_cast<double (&)[3][1]>(pos_x)),
      pos_y_side_normal(const_cast<double (&)[3][1]>(pos_y)),
      neg_x_side_normal(const_cast<double (&)[3][1]>(neg_x)),
      neg_y_side_normal(const_cast<double (&)[3][1]>(neg_x)),
      neg_z_side_normal(const_cast<double (&)[3][1]>(neg_z)),
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
    z_component.MultiplyScalar(z_component, -0.5);

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

bool EarthSensor::ReadSensors() {
    // TODO(rskew): I2C read using ir-driver/InfraredSensor

    // TODO (rskew) calibrate IR readings

    InfraredReading infrared_reading;
    bool success;
    if (hil_enabled) {
        success = ReadSensorsHil(infrared_reading);
        // Echo reading to data dashboard
        RunnableDataDashboard::TransmitMessage(
            kInfraredReadingCode, InfraredReading_size, InfraredReading_fields,
            &infrared_reading);
    }

    //Log_warning2("Infrared reading received:\npos_x: %d\tneg_x: %d",
    //infrared_reading.positive_x,
    //infrared_reading.negative_x);
    //Log_warning2("pos_y: %d\tneg_y: %d", infrared_reading.positive_y,
    //infrared_reading.negative_y);
    //Log_warning2("neg_z_a: %d\tneg_z_b: %d", infrared_reading.negative_z_a,
    //infrared_reading.negative_z_b);

    // TODO (rskew) combine noise from hardware readings with simulated readings

    SetPosXSensorReading(infrared_reading.positive_x);
    SetPosYSensorReading(infrared_reading.positive_y);
    SetNegXSensorReading(infrared_reading.negative_x);
    SetNegXSensorReading(infrared_reading.negative_y);
    SetNegZASensorReading(infrared_reading.negative_z_a);
    SetNegZBSensorReading(infrared_reading.negative_z_b);

    CalculateNadirVector();

    return success;
}

bool EarthSensor::ReadSensorsHil(InfraredReading &infrared_reading) {
    DebugStream *debug_stream = DebugStream::GetInstance();
    uint8_t buffer[InfraredReading_size];

    bool success = debug_stream->RequestMessageFromSimulator(
        kInfraredReadingRequestCode, buffer, InfraredReading_size);

    if (success) {
        pb_istream_t stream =
            pb_istream_from_buffer(buffer, InfraredReading_size);

        bool status = pb_decode(&stream, InfraredReading_fields,
                                &infrared_reading);
        if (!status) {
            etl::exception e("pb_decode failed", __FILE__, __LINE__);
            throw e;
        }
    }
    return success;
}
