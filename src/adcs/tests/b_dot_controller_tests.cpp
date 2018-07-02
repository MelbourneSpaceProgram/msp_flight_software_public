#include <CppUTest/TestHarness.h>
#include <src/adcs/controllers/b_dot_controller.h>
#include <src/util/matrix.h>

TEST_GROUP(BDotController){};

TEST(BDotController, SaturationInEachAxis) {
    double b_data[3][1] = {{10}, {-10}, {10}};
    Matrix b(b_data);
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double torque_data[3][1];
    Matrix torque(torque_data);

    BDotController::Control(b, b_dot, torque);

    DOUBLES_EQUAL(4, torque.Get(0, 0), 0.01);
    DOUBLES_EQUAL(-3, torque.Get(1, 0), 0.01);
    DOUBLES_EQUAL(-7, torque.Get(2, 0), 0.01);
}

TEST(BDotController, TorqueInExpectedDirection) {
    double b_data[3][1] = {{10}, {-10}, {10}};
    Matrix b(b_data);
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double torque_data[3][1];
    Matrix torque(torque_data);

    b_dot.Set(0, 0, -0.01);
    b_dot.Set(1, 0, 0);
    b_dot.Set(2, 0, 0);
    b.Set(0, 0, 0);
    b.Set(1, 0, 100);
    b.Set(2, 0, 0);

    BDotController::Control(b, b_dot, torque);

    DOUBLES_EQUAL(0, torque.Get(0, 0), 0.01);
    DOUBLES_EQUAL(0, torque.Get(1, 0), 0.01);
    DOUBLES_EQUAL(35, torque.Get(2, 0), 0.01);
}

TEST(BDotController, InvalidInputSize) {
    double b_data[3][1] = {{10}, {-10}, {10}};
    Matrix b(b_data);
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double torque_data[3][1];
    Matrix torque(torque_data);
    double m32_data[3][2];
    Matrix m32(m32_data);

    CHECK_THROWS(etl::exception, BDotController::Control(m32, b_dot, torque));
    CHECK_THROWS(etl::exception, BDotController::Control(b, m32, torque));
}
