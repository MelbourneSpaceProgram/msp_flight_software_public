#ifndef SRC_SENSORS_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_
#define SRC_SENSORS_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_

#include <string>
#include <vector>
#include "../generic_measurable.h"

class SoftwareSensor : public GenericMeasurable<double> {
   public:
    explicit SoftwareSensor(std::string id);
    std::string GetId() const;
    void LoadReading(double reading);
    double GetReading();

   protected:
    std::string id;
    bool initialised;
};

#endif  //  SRC_SENSORS_SOFTWARE_SENSOR_SOFTWARE_SENSOR_H_
