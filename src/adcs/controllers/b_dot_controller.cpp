#include <external/etl/exception.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/util/matrix.h>
#include <src/util/physical_constants.h>
#include <src/adcs/magnetorquer_driver>

void BDotController::Control(const Matrix &b, const Matrix &b_dot,
                             Matrix &pwm_output) {
    if (!b_dot.SameSize(b) || !b_dot.SameSize(pwm_output) ||
        b_dot.GetNRows() != geomagnetic_field_vector_nrows ||
        b_dot.GetNColumns() != geomagnetic_field_vector_ncolumns) {
        etl::exception e("BDotController::Control arguments not 3x1", __FILE__,
                         __LINE__);
        throw e;
    }
    // Calculate the magnetic dipole to produce
    for (uint8_t i = 0; i < 3; i++) {
        int sign = ((b_dot.Get(i, 0) > 0) - (b_dot.Get(i, 0) < 0));
        pwm_output.Set(
            i, 0, -(static_cast<double>(sign)));
    }
    MagnetorquerControl::SetMagnetorquersPowerFraction(
                                                        pwm_output.Get(0,0)
                                                       ,pwm_output.Get(1,0)
                                                       ,pwm_output.Get(2,0));


}
