#ifndef SRC_SYSTEM_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_
#define SRC_SYSTEM_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_

#include <src/sensors/generic_sensor.h>
#include <string>
#include <vector>

class SoftwareSensor : public GenericSensor<double> {
public:
    explicit SoftwareSensor(std::string id);
    std::string GetId() const;
    void SetReading(double reading);
    void TakeReading();

protected:
    std::string id;
    double reading;
};

#endif  //  SRC_SYSTEM_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_
