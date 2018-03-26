#include <src/sensors/test_sensors/test_int_sensor.h>

TestIntSensor::TestIntSensor() {}

bool TestIntSensor::TakeReading() {
    reading = dummy_data;
    NotifyObservers();
    return true;
}

void TestIntSensor::SetDummySensorData(int dummy_data) {
    this->dummy_data = dummy_data;
}
