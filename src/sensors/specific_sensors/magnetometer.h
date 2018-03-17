#ifndef SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
#define SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_

#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/generic_sensor.h>
#include <string>

class Magnetometer : public GenericSensor<MagnetometerReading> {
   public:
    explicit Magnetometer(std::string id);
    bool TakeReading();

   private:
    std::string id;
    MagnetometerReading reading;
    bool TakeReadingHil();
};

#endif  //  SRC_SENSORS_SPECIFIC_SENSORS_MAGNETOMETER_H_
