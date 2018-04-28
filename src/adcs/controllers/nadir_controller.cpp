//#include <src/adcs/controllers/nadir_controller.h>
//
//const double NadirController::max_achievable_dipole[] = {0.35, 0.35, 0.05};
//const double NadirController::kp = 0.02;
//const double NadirController::kd = 7.0;
//
//void NadirController::Control(const Matrix &nadir_error_quart, Matrix &omega,
//                              Matrix &torque_output) {
//    double error_vector_data[3][1];
//    for (int i = 0; i < 3; i++) {
//        error_vector_data[i][0] = nadir_error_quart.Get(i + 1, 0);
//    }
//    Matrix error_vector(error_vector_data);
//    error_vector.MultiplyScalar(error_vector, -1 * kp);
//    omega.MultiplyScalar(omega, -1 * kd);
//    torque_output.Add(omega, error_vector);
//}
