#include <external/etl/exception.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/config/unit_tests.h>
#include <src/data_dashboard/runnable_data_dashboard.h>
#include <src/messages/TorqueOutputReading.pb.h>
#include <src/util/matrix.h>
#include <src/util/message_codes.h>
#include <src/util/physical_constants.h>

void BDotController::ComputeControl(const Matrix &b, const Matrix &b_dot,
                                    Matrix &signed_pwm_output) {
    if (!b_dot.SameSize(b) || !b_dot.SameSize(signed_pwm_output) ||
        b_dot.GetNRows() != geomagnetic_field_vector_nrows ||
        b_dot.GetNColumns() != geomagnetic_field_vector_ncolumns) {
        etl::exception e("BDotController::Control arguments not 3x1", __FILE__,
                         __LINE__);
        throw e;
    }
    // Calculate the magnetic dipole to produce
    for (uint8_t i = 0; i < 3; i++) {
        int sign = ((b_dot.Get(i, 0) > 0) - (b_dot.Get(i, 0) <= 0));
        signed_pwm_output.Set(i, 0, -(static_cast<double>(sign)));
    }

    if (hil_available) {
        double max_achievable_dipole[3] = {0.35, 0.35, 0.05};
        // Calculate the magnetic dipole to produce
        double magnetic_dipole_data[3][1];
        Matrix magnetic_dipole(magnetic_dipole_data);
        for (uint8_t i = 0; i < 3; i++) {
            int sign = ((b_dot.Get(i, 0) > 0) - (b_dot.Get(i, 0) < 0));
            magnetic_dipole.Set(
                i, 0, -(static_cast<double>(sign)) * max_achievable_dipole[i]);
        }
        // Calculate torque with cross product
        double torque_output_data[3][1];
        Matrix torque_output(torque_output_data);
        torque_output.CrossProduct(magnetic_dipole, b);

        // Echo reading to data dashboard
        TorqueOutputReading torque_output_reading =
            TorqueOutputReading_init_zero;
        torque_output_reading.x = torque_output.Get(0, 0);
        torque_output_reading.y = torque_output.Get(1, 0);
        torque_output_reading.z = torque_output.Get(2, 0);
        RunnableDataDashboard::TransmitMessage(
            kTorqueOutputReadingCode, TorqueOutputReading_size,
            TorqueOutputReading_fields, &torque_output_reading);
    }
}
