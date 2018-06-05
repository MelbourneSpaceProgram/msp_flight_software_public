#ifndef SRC_ADCS_STATE_ESTIMATORS_B_DOT_ESTIMATOR_H_
#define SRC_ADCS_STATE_ESTIMATORS_B_DOT_ESTIMATOR_H_

#include <src/util/data_types.h>
#include <src/util/finite_difference_symmetric.h>
#include <src/util/first_order_iir_lowpass.h>

class Matrix;

class BDotEstimator {
   public:
    BDotEstimator(uint16_t sample_period_millis, uint16_t time_constant_millis);
    void Estimate(const Matrix &magnetometer_reading, Matrix &estimate_output);

   private:
    FirstOrderIirLowpass smoother_x;
    FirstOrderIirLowpass smoother_y;
    FirstOrderIirLowpass smoother_z;
    FiniteDifferenceSymmetric differentiator_x;
    FiniteDifferenceSymmetric differentiator_y;
    FiniteDifferenceSymmetric differentiator_z;
};

#endif  // SRC_ADCS_STATE_ESTIMATORS_B_DOT_ESTIMATOR_H_
