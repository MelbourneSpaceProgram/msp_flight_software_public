#include <src/adcs/controllers/nadir_controller.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/util/matrix.h>
#include <src/util/physical_constants.h>

void NadirController::ComputeControl(Matrix &error_quart, Matrix &omega,
                                     Matrix &torque_output) {
    NewStackMatrixMacro(error_vector, 3, 1);
    int sign = (error_quart.Get(3, 0) >= 0) - (error_quart.Get(3, 0) < 0);
    for (int i = 0; i < 3; i++) {
        error_vector.Set(i, 0, error_quart.Get(i + 1, 0));
    }
    error_vector.MultiplyScalar(error_vector, -1 * kNadirControllerKp);

    // If we're the wrong way up, push to increase omega to flip around quicker
    // TODO (JmcRobbie) demonstrate that this is what happens in simulation
    NewStackMatrixMacro(derivative_error, 3, 1);
    derivative_error.MultiplyScalar(omega, -sign * kNadirControllerKd);

    torque_output.Add(derivative_error, error_vector);
}
