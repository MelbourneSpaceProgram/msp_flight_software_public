#ifndef SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
#define SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_

#include <src/messages/MagnetometerReading.pb.h>
#include <string>
#include "../generic_measurable.h"

class Magnetometer : public GenericMeasurable<MagnetometerReading> {
   public:
    explicit Magnetometer(std::string id);
    bool TakeReading();

   private:
    std::string id;
    MagnetometerReading reading;
    bool TakeReadingHil();
};

#endif  //  SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
