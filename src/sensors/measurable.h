#ifndef SRC_SENSORS_MEASURABLE_H_
#define SRC_SENSORS_MEASURABLE_H_

#include <external/etl/vector.h>

class Measurable {
   public:
    virtual ~Measurable(){};
    virtual bool TakeReading() = 0;

   protected:
    Measurable(){};
};

#endif  //  SRC_SENSORS_MEASURABLE_H_
