#ifndef SRC_SENSORS_SPECIFIC_SENSORS_GYROMETER_H_
#define SRC_SENSORS_SPECIFIC_SENSORS_GYROMETER_H_

#include <src/messages/GyrometerReading.pb.h>
#include <src/sensors/generic_measurable.h>

class Gyrometer : public GenericMeasurable<GyrometerReading> {
   public:
    explicit Gyrometer(){};
    bool TakeReading();

   private:
    bool TakeReadingHil();
};

#endif  //  SRC_SENSORS_SPECIFIC_SENSORS_GYROMETER_H_
