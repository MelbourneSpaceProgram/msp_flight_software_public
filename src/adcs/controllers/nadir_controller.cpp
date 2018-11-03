#include <src/adcs/controllers/nadir_controller.h>
#include <src/util/matrix.h>
#include <src/util/physical_constants.h>

const double NadirController::max_achievable_dipole[] = {0.35, 0.35, 0.05};
const double NadirController::kp = 0.02;
const double NadirController::kd = 7.0;

void NadirController::Control(const Matrix &nadir_error_quart, Matrix &omega,
                              Matrix &torque_output) {
    NewStackMatrixMacro(error_vector, 3, 1);
    int sign = (nadir_error_quart.Get(3,0)>=0)-(nadir_error_quart.Get(3,0)<0);
    for (int i = 0; i < 3; i++) {
        error_vector.Set(i, 0, nadir_error_quart.Get(i + 1, 0));
    }
    error_vector.MultiplyScalar(error_vector, -1 * kp);
    omega.MultiplyScalar(omega, -sign * kd);
    torque_output.Add(omega, error_vector);
}
