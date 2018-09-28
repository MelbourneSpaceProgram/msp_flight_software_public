#ifndef SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
#define SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_

class Matrix;

class BDotController {
   public:
    static void ComputeControl(const Matrix &b_dot,
                               Matrix &signed_pwm_output);
    static const double ControllerGains[3];
    static const double max_dipole[3];
};

#endif  // SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
