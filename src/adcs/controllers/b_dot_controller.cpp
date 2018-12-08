#include <math.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/config/satellite.h>
#include <src/util/matrix.h>
#include <src/util/msp_exception.h>
#include <src/util/physical_constants.h>
#include <algorithm>

void BDotController::ComputeControl(const Matrix &b_dot,
                                    Matrix &signed_pwm_output) {
    if (!b_dot.SameSize(signed_pwm_output) ||
        b_dot.GetNRows() != geomagnetic_field_vector_nrows ||
        b_dot.GetNColumns() != geomagnetic_field_vector_ncolumns) {
        throw MspException("BDotController::Control arguments not 3x1",
                           kBdotControllerArgumentFail, __FILE__, __LINE__);
    }

    /* Tuning Parameters - estimated and optimised via simulation and testing.
     */

    double result[3];
    double result_abs[3];
    for (uint8_t i = 0; i < 3; i++) {
        result[i] = -1 * kBDotControllerGains[i] * b_dot.Get(i, 0) /
                    SystemConfiguration::GetInstance()->GetMaxMagnetorquerDipole()[i];
        result_abs[i] = fabs(result[i]);
    }
    if (result_abs[0] > 1 || result_abs[1] > 1 || result_abs[2] > 1) {
        // proportional control needs to be scaled to preserve dipole direction

        /*Returns the index of the largest value of the array*/

        double max = *std::max_element(
            result_abs, result_abs + sizeof(result_abs) / sizeof(double));

        for (int i = 0; i < 3; i++) {
            signed_pwm_output.Set(i, 0, result[i] / max);
        }
        return;
    } else {
        // normal proportional control
        signed_pwm_output.Set(0, 0, result[0]);
        signed_pwm_output.Set(1, 0, result[1]);
        signed_pwm_output.Set(2, 0, result[2]);
        return;
    }
}
