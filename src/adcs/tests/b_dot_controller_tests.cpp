#include <CppUTest/TestHarness.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/util/matrix.h>

TEST_GROUP(BDotController){};

TEST(BDotController, SaturationInEachAxis) {
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double signed_pwm_data[3][1];
    Matrix signed_pwm(signed_pwm_data);

    BDotController::ComputeControl( b_dot, signed_pwm);

    DOUBLES_EQUAL(0.25, signed_pwm.Get(0, 0), 0.01);
    DOUBLES_EQUAL(0.25, signed_pwm.Get(1, 0), 0.01);
    DOUBLES_EQUAL(1, signed_pwm.Get(2, 0), 0.01);
}

TEST(BDotController, SignedPwmInExpectedDirection) {
    double b_dot_data[3][1];
    Matrix b_dot(b_dot_data);
    double signed_pwm_data[3][1];
    Matrix signed_pwm(signed_pwm_data);
    double bdot_est = 1E-6;
    b_dot.Set(0, 0, bdot_est);
    b_dot.Set(1, 0, bdot_est);
    b_dot.Set(2, 0, bdot_est*-1);

    BDotController::ComputeControl( b_dot, signed_pwm);

    DOUBLES_EQUAL(-bdot_est*BDotController::ControllerGains[0]/BDotController::max_dipole[0], signed_pwm.Get(0, 0), 0.0001);
    DOUBLES_EQUAL(-bdot_est*BDotController::ControllerGains[1]/BDotController::max_dipole[1], signed_pwm.Get(1, 0), 0.0001);
    DOUBLES_EQUAL(bdot_est*BDotController::ControllerGains[2]/BDotController::max_dipole[2], signed_pwm.Get(2, 0), 0.0001);

    b_dot.Set(0, 0, 10*bdot_est);
    b_dot.Set(1, 0, bdot_est);
    b_dot.Set(2, 0, bdot_est*-1);

    BDotController::ComputeControl( b_dot, signed_pwm);


    DOUBLES_EQUAL(-1, signed_pwm.Get(0, 0), 0.001);

    // the divide by 1.5 factor is included because it is the correct scaling of the output.

    DOUBLES_EQUAL(-bdot_est*BDotController::ControllerGains[1]/BDotController::max_dipole[1]/1.5, signed_pwm.Get(1, 0), 0.0001);
    DOUBLES_EQUAL(bdot_est*BDotController::ControllerGains[2]/BDotController::max_dipole[2]/1.5, signed_pwm.Get(2, 0), 0.0001);

}

TEST(BDotController, InvalidInputSize) {
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double signed_pwm_data[3][1];
    Matrix signed_pwm(signed_pwm_data);
    double m32_data[3][2];
    Matrix m32(m32_data);

    CHECK_THROWS(etl::exception,
                 BDotController::ComputeControl(m32, signed_pwm));
    CHECK_THROWS(etl::exception, BDotController::ComputeControl(b_dot, m32));
}
