#include <src/sensors/test_sensors/test_int_sensor.h>

TestIntSensor::TestIntSensor(std::string id) : id(id) {}

bool TestIntSensor::TakeReading() {
    reading = dummy_data;
    NotifyObservers();
    return true;
}

void TestIntSensor::SetDummySensorData(int dummy_data) {
    this->dummy_data = dummy_data;
}
