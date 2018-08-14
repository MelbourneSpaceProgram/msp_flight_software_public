#include <CppUTest/TestHarness.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/util/matrix.h>

TEST_GROUP(BDotController){};

TEST(BDotController, SaturationInEachAxis) {
    double b_data[3][1] = {{10}, {-10}, {10}};
    Matrix b(b_data);
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double signed_pwm_data[3][1];
    Matrix signed_pwm(signed_pwm_data);

    BDotController::ComputeControl(b, b_dot, signed_pwm);

    DOUBLES_EQUAL(1, signed_pwm.Get(0, 0), 0.01);
    DOUBLES_EQUAL(1, signed_pwm.Get(1, 0), 0.01);
    DOUBLES_EQUAL(1, signed_pwm.Get(2, 0), 0.01);
}

TEST(BDotController, SignedPwmInExpectedDirection) {
    double b_data[3][1];
    Matrix b(b_data);
    double b_dot_data[3][1];
    Matrix b_dot(b_dot_data);
    double signed_pwm_data[3][1];
    Matrix signed_pwm(signed_pwm_data);

    b_dot.Set(0, 0, -0.01);
    b_dot.Set(1, 0, 0.01);
    b_dot.Set(2, 0, 0.01);
    b.Set(0, 0, 0);
    b.Set(1, 0, 100);
    b.Set(2, 0, 0);

    BDotController::ComputeControl(b, b_dot, signed_pwm);

    DOUBLES_EQUAL(1, signed_pwm.Get(0, 0), 0.01);
    DOUBLES_EQUAL(-1, signed_pwm.Get(1, 0), 0.01);
    DOUBLES_EQUAL(-1, signed_pwm.Get(2, 0), 0.01);
}

TEST(BDotController, InvalidInputSize) {
    double b_data[3][1] = {{10}, {-10}, {10}};
    Matrix b(b_data);
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double signed_pwm_data[3][1];
    Matrix signed_pwm(signed_pwm_data);
    double m32_data[3][2];
    Matrix m32(m32_data);

    CHECK_THROWS(etl::exception,
                 BDotController::ComputeControl(m32, b_dot, signed_pwm));
    CHECK_THROWS(etl::exception,
                 BDotController::ComputeControl(b, m32, signed_pwm));
    CHECK_THROWS(etl::exception, BDotController::ComputeControl(b, b_dot, m32));
}
