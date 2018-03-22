#ifndef SRC_UTIL_FINITE_DIFFERENCE_SYMMETRIC_H_
#define SRC_UTIL_FINITE_DIFFERENCE_SYMMETRIC_H_

#include <src/util/data_types.h>

class FiniteDifferenceSymmetric {
   public:
    FiniteDifferenceSymmetric(uint16_t sample_period_millis);

    double ProcessSample(double new_sample);

   private:
    double sample_period_seconds;
    double sample_one_ago;
    double sample_two_ago;
};

#endif  // SRC_UTIL_FINITE_DIFFERENCE_SYMMETRIC_H_
