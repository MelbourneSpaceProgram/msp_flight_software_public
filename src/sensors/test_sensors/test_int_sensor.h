#ifndef SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_
#define SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_

#include <src/sensors/generic_sensor.h>
#include <string>

class TestIntSensor : public GenericSensor<int> {
   public:
    explicit TestIntSensor();
    bool TakeReading();
    void SetDummySensorData(int dummy_dat);

   private:
    int dummy_data;
};

#endif  //  SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_
