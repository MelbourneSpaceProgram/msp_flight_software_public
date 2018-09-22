#include <src/sensors/test_sensors/test_i2c_measurable.h>

TestI2cMeasurable::TestI2cMeasurable()
    : I2cMeasurable<TestI2cReading>(NULL, {0, 0}), data({0, 0}) {}

TestI2cReading TestI2cMeasurable::TakeDirectI2cReading() { return data; }

void TestI2cMeasurable::SetDummySensorData(double dummy_data) {
    data.dummy_data = dummy_data;
}
