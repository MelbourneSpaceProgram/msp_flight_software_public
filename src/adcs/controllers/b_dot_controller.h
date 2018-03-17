#ifndef SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
#define SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_

#include <src/util/matrix.h>

class BDotController {
   public:
    static void Control(const Matrix &b, const Matrix &b_dot,
                        Matrix &torque_output);

   private:
    static const double max_achievable_dipole[3];
    static const uint8_t input_vector_rows = 3;
    static const uint8_t input_vector_columns = 1;
};

#endif  // SRC_ADCS_CONTROLLERS_B_DOT_CONTROLLER_H_
