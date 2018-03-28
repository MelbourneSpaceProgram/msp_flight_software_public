#ifndef SRC_SENSORS_TEST_SENSORS_TEST_I2C_SENSOR_H_
#define SRC_SENSORS_TEST_SENSORS_TEST_I2C_SENSOR_H_

#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <string>

class TestI2cSensor : public I2cSensor<double> {
   public:
    explicit TestI2cSensor();
    double TakeI2cReading();
    void SetDummySensorData(double dummy_data);

   private:
    double dummy_data;
    static const uint8_t kTestI2cAddress = 0x00;
};

#endif  //  SRC_SENSORS_TEST_SENSORS_TEST_I2C_SENSOR_H_
