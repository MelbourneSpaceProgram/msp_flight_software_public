#include <src/sensors/test_sensors/test_int_sensor.h>

TestIntSensor::TestIntSensor() {}

bool TestIntSensor::TakeReading() {
    last_reading = dummy_data;
    return true;
}

void TestIntSensor::SetDummySensorData(int dummy_data) {
    this->dummy_data = dummy_data;
}
