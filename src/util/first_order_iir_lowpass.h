#ifndef SRC_UTIL_FIRST_ORDER_IIR_LOWPASS_H_
#define SRC_UTIL_FIRST_ORDER_IIR_LOWPASS_H_

#include <src/util/data_types.h>

class FirstOrderIirLowpass {
   public:
    FirstOrderIirLowpass(uint16_t time_constant_millis,
                         uint16_t sample_period_millis);

    double ProcessSample(double new_sample);

   private:
    double state;
    double alpha;
};

#endif  // SRC_UTIL_FIRST_ORDER_IIR_LOWPASS_H_
