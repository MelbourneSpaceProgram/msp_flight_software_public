//#include <src/adcs/controllers/b_dot_controller.h>
//#include <src/util/physical_constants.h>
//
//const double BDotController::max_achievable_dipole[] = {0.35, 0.35, 0.05};
//
//void BDotController::Control(const Matrix &b, const Matrix &b_dot,
//                             Matrix &torque_output) {
//    if (!b_dot.SameSize(b) || !b_dot.SameSize(torque_output) ||
//        b_dot.GetNRows() != geomagnetic_field_vector_nrows ||
//        b_dot.GetNColumns() != geomagnetic_field_vector_ncolumns) {
//        etl::exception e("BDotController::Control arguments not 3x1",
//                         __FILE__, __LINE__);
//        throw e;
//    }
//    // Calculate the magnetic dipole to produce
//    double magnetic_dipole_data[3][1];
//    Matrix magnetic_dipole(magnetic_dipole_data);
//    for (uint8_t i = 0; i < 3; i++) {
//        int sign = ((b_dot.Get(i, 0) > 0) - (b_dot.Get(i, 0) < 0));
//        magnetic_dipole.Set(
//            i, 0, -(static_cast<double>(sign)) * max_achievable_dipole[i]);
//    }
//
//    // Calculate torque with cross product
//    torque_output.CrossProduct(magnetic_dipole, b);
//}
