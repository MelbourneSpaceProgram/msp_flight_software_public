#ifndef SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
#define SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_

class Matrix;

class BDotController {
   public:
    static void ComputeControl(const Matrix &b, const Matrix &b_dot,
                               Matrix &signed_pwm_output);
};

#endif  // SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
