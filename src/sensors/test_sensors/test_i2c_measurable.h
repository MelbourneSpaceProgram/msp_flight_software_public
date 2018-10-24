#ifndef SRC_SENSORS_TEST_SENSORS_TEST_I2C_SENSOR_H_
#define SRC_SENSORS_TEST_SENSORS_TEST_I2C_SENSOR_H_

#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

typedef struct {
    double dummy_data;
    uint64_t timestamp_ms;
} TestI2cReading;

class TestI2cMeasurable : public I2cMeasurable<TestI2cReading> {
   public:
    explicit TestI2cMeasurable();
    TestI2cReading TakeDirectI2cReading();
    void SetDummySensorData(double dummy_data);

   private:
    TestI2cReading data;
    static constexpr uint8_t kTestI2cAddress = 0x00;
};

#endif  //  SRC_SENSORS_TEST_SENSORS_TEST_I2C_SENSOR_H_
