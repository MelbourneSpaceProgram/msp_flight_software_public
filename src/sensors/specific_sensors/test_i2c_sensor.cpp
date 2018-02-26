#include <src/sensors/specific_sensors/test_i2c_sensor.h>
#include <src/observers/observer.h>

TestI2cSensor::TestI2cSensor(std::string id) : id(id) {}

void TestI2cSensor::TakeReading() {
    reading = dummy_data;
    new_reading = true;
    NotifyObservers();
    new_reading = false;
}

void TestI2cSensor::SetDummySensorData(int dummy_data) {
    this->dummy_data = dummy_data;
}
