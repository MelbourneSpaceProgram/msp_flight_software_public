#ifndef SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
#define SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_

#include <src/util/data_types.h>

class Matrix;

class BDotController {
   public:
    static void ComputeControl(const Matrix &b_dot, Matrix &signed_pwm_output);
    static double Maximum(double arr[], uint16_t size);
};

#endif  // SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
