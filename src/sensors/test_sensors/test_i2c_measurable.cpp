#include <src/sensors/test_sensors/test_i2c_measurable.h>

TestI2cMeasurable::TestI2cMeasurable() : I2cMeasurable<double>(NULL, -9999) {}

double TestI2cMeasurable::TakeDirectI2cReading() { return dummy_data; }

void TestI2cMeasurable::SetDummySensorData(double dummy_data) {
    this->dummy_data = dummy_data;
}
