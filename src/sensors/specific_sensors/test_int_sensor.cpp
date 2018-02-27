#include <src/sensors/specific_sensors/test_int_sensor.h>

TestIntSensor::TestIntSensor(std::string id) : id(id) {}

void TestIntSensor::TakeReading() {
    reading = dummy_data;
    new_reading = true;
    NotifyObservers();
    new_reading = false;
}

void TestIntSensor::SetDummySensorData(int dummy_data) {
    this->dummy_data = dummy_data;
}
