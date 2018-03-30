#ifndef SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_
#define SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_

#include <src/sensors/generic_measurable.h>
#include <string>

class TestIntSensor : public GenericMeasurable<int> {
   public:
    explicit TestIntSensor();
    bool TakeReading();
    void SetDummySensorData(int dummy_dat);

   private:
    int dummy_data;
};

#endif  //  SRC_SENSORS_TEST_SENSORS_TEST_INT_SENSOR_H_
