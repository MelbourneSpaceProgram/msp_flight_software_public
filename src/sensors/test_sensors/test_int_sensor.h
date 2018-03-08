#ifndef SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_
#define SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_

#include <src/sensors/generic_sensor.h>
#include <string>

class TestIntSensor : public GenericSensor<int> {
   public:
    explicit TestIntSensor(std::string id);
    bool TakeReading();
    void SetDummySensorData(int dummy_dat);

   private:
    int dummy_data;
    std::string id;
};

#endif  //  SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_
