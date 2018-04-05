#ifndef SRC_OBSERVERS_SENSOR_CONTAINER_H_
#define SRC_OBSERVERS_SENSOR_CONTAINER_H_

#include <external/etl/exception.h>
#include <external/etl/vector.h>

template <typename T1, typename T2>
class SensorContainer {
   public:
    bool AddSensor(T1* sensor, T2 observer) {
        if (sensors.available()) {
            sensor->AddObserver(observer);
            sensors.push_back(sensor);
            return true;
        } else {
            etl::exception e("No capacity to add sensor", "__FILE__",
                         __LINE__);
            throw e;
            return false;
        }
    }

   protected:
    T1* GetSensorWithReading() {
        for (typename etl::vector<T1*, kMaxSensors>::iterator it =
                 sensors.begin();
             it != sensors.end(); it++) {
            if ((*it)->HasNewReading()) {
                return (*it);
            }
        }
        return NULL;
    }
    static const uint8_t kMaxSensors = 3;
    etl::vector<T1*, kMaxSensors> sensors;
};

#endif  //  SRC_OBSERVERS_SENSOR_CONTAINER_H_
