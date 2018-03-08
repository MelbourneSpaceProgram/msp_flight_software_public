#ifndef SRC_SENSORS_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_
#define SRC_SENSORS_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_

#include <src/sensors/generic_sensor.h>
#include <string>
#include <vector>

class SoftwareSensor : public GenericSensor<double> {
public:
    explicit SoftwareSensor(std::string id);
    std::string GetId() const;
    void LoadReading(double reading);
    bool TakeReading();

protected:
    std::string id;
    double reading;
    bool initialised;
};

#endif  //  SRC_SENSORS_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_
