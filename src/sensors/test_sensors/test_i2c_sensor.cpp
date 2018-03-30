#include <src/sensors/test_sensors/test_i2c_sensor.h>

TestI2cSensor::TestI2cSensor() : I2cMeasurable<double>(NULL) {}

double TestI2cSensor::TakeI2cReading() { return dummy_data; }

void TestI2cSensor::SetDummySensorData(double dummy_data) {
    this->dummy_data = dummy_data;
}
