#include <CppUTest/TestHarness.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/config/orientation_control_tuning_parameters.h>
#include <src/config/satellite.h>
#include <src/util/matrix.h>

TEST_GROUP(BDotController){};

TEST(BDotController, SaturationInEachAxis) {
    NewStackMatrixMacro(b_dot, 3, 1);
    b_dot.Fill(-100);
    NewStackMatrixMacro(signed_pwm, 3, 1);

    BDotController::ComputeControl(b_dot, signed_pwm);

    DOUBLES_EQUAL(0.25, signed_pwm.Get(0, 0), 0.01);
    DOUBLES_EQUAL(0.25, signed_pwm.Get(1, 0), 0.01);
    DOUBLES_EQUAL(1, signed_pwm.Get(2, 0), 0.01);
}

TEST(BDotController, SignedPwmInExpectedDirection) {
    NewStackMatrixMacro(b_dot, 3, 1);
    NewStackMatrixMacro(signed_pwm, 3, 1);
    double bdot_est = 1E-6;
    b_dot.Set(0, 0, bdot_est);
    b_dot.Set(1, 0, bdot_est);
    b_dot.Set(2, 0, bdot_est * -1);

    BDotController::ComputeControl(b_dot, signed_pwm);

    DOUBLES_EQUAL(
        -bdot_est * kBDotControllerGains[0] / kMaxMagnetorquerDipole[0],
        signed_pwm.Get(0, 0), 0.0001);
    DOUBLES_EQUAL(
        -bdot_est * kBDotControllerGains[1] / kMaxMagnetorquerDipole[1],
        signed_pwm.Get(1, 0), 0.0001);
    DOUBLES_EQUAL(
        bdot_est * kBDotControllerGains[2] / kMaxMagnetorquerDipole[2],
        signed_pwm.Get(2, 0), 0.0001);

    b_dot.Set(0, 0, 10 * bdot_est);
    b_dot.Set(1, 0, bdot_est);
    b_dot.Set(2, 0, bdot_est * -1);

    BDotController::ComputeControl(b_dot, signed_pwm);

    DOUBLES_EQUAL(-1, signed_pwm.Get(0, 0), 0.001);

    // the divide by 1.5 factor is included because it is the correct scaling of
    // the output.

    DOUBLES_EQUAL(
        -bdot_est * kBDotControllerGains[1] / kMaxMagnetorquerDipole[1] / 1.5,
        signed_pwm.Get(1, 0), 0.0001);
    DOUBLES_EQUAL(
        bdot_est * kBDotControllerGains[2] / kMaxMagnetorquerDipole[2] / 1.5,
        signed_pwm.Get(2, 0), 0.0001);
}

TEST(BDotController, InvalidInputSize) {
    NewStackMatrixMacro(b_dot, 3, 1);
    b_dot.Fill(-100);
    NewStackMatrixMacro(signed_pwm, 3, 1);
    NewStackMatrixMacro(m32, 3, 2);

    CHECK_THROWS(MspException,
                 BDotController::ComputeControl(m32, signed_pwm));
    CHECK_THROWS(MspException, BDotController::ComputeControl(b_dot, m32));
}
