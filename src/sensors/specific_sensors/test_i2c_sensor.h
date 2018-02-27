#ifndef SRC_SENSORS_SPECIFIC_SENSORS_TEST_I2C_SENSOR_H_
#define SRC_SENSORS_SPECIFIC_SENSORS_TEST_I2C_SENSOR_H_

#include <src/sensors/generic_sensor.h>
#include <string>

class TestI2cSensor : public GenericSensor<double> {
   public:
    explicit TestI2cSensor(std::string id);
    void TakeReading();
    void SetDummySensorData(double dummy_data);

   private:
    double dummy_data;
    std::string id;
};

#endif  //  SRC_SENSORS_SPECIFIC_SENSORS_TEST_I2C_SENSOR_H_
