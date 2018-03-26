#include <src/sensors/test_sensors/test_i2c_sensor.h>

TestI2cSensor::TestI2cSensor() {}

bool TestI2cSensor::TakeReading() {
    reading = dummy_data;
    NotifyObservers();
    return true;
}

void TestI2cSensor::SetDummySensorData(double dummy_data) {
    this->dummy_data = dummy_data;
}
