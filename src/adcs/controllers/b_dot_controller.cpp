#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/config/satellite.h>
#include <src/util/matrix.h>
#include <src/util/msp_exception.h>
#include <src/util/physical_constants.h>

void BDotController::ComputeControl(const Matrix &b_dot,
                                    Matrix &signed_pwm_output) {
    // Check input dimensions
    if (!b_dot.SameSize(signed_pwm_output) ||
        b_dot.GetNRows() != geomagnetic_field_vector_nrows ||
        b_dot.GetNColumns() != geomagnetic_field_vector_ncolumns) {
        throw MspException("BDotController::Control arguments not 3x1",
                           kBdotControllerArgumentFail, __FILE__, __LINE__);
    }

    double control_pwm[3];
    double control_pwm_abs[3];
    for (uint8_t i = 0; i < 3; i++) {
        control_pwm[i] = -1 * kBDotControllerGains[i] * b_dot.Get(i, 0) /
                         kMaxMagnetorquerDipole[i];
        control_pwm_abs[i] = fabs(control_pwm[i]);
    }

    double largest_pwm_abs = Maximum(control_pwm_abs, 3);
    // Always saturate control vector for Helmholtz test
    // if (largest_pwm > 1) {
    // Control vector needs to be clipped to preserve dipole direction.
    // Scale the control vector by the largest PWM value to bring it back
    // inside the box of realisable control outputs.

    for (int i = 0; i < 3; i++) {
        signed_pwm_output.Set(i, 0, control_pwm[i] / largest_pwm_abs);
    }
    return;
    //} else {
    //    // Normal proportional control
    //    signed_pwm_output.Set(0, 0, control_pwm[0]);
    //    signed_pwm_output.Set(1, 0, control_pwm[1]);
    //    signed_pwm_output.Set(2, 0, control_pwm[2]);
    //    return;
    //}
}

double BDotController::Maximum(double arr[], uint16_t size) {
    double max = arr[0];
    for (uint16_t i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}
