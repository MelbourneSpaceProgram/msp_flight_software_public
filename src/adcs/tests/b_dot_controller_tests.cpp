#include <src/adcs/controllers/b_dot_controller.h>
#include <src/util/matrix.h>
#include <test_runners/unity.h>

void TestControl() {
    // test saturation in each axis
    double b_data[3][1] = {{10}, {-10}, {10}};
    Matrix b(b_data);
    double b_dot_data[3][1] = {{-100}, {-100}, {-100}};
    Matrix b_dot(b_dot_data);
    double torque_data[3][1];
    Matrix torque(torque_data);

    BDotController::Control(b, b_dot, torque);

    TEST_ASSERT_EQUAL_DOUBLE(4, torque.Get(0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-3, torque.Get(1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(-7, torque.Get(2, 0));

    // test torque is correctish direction
    b_dot.Set(0, 0, -0.01);
    b_dot.Set(1, 0, 0);
    b_dot.Set(2, 0, 0);
    b.Set(0, 0, 0);
    b.Set(1, 0, 100);
    b.Set(2, 0, 0);

    BDotController::Control(b, b_dot, torque);

    TEST_ASSERT_EQUAL_DOUBLE(0, torque.Get(0, 0));
    TEST_ASSERT_EQUAL_DOUBLE(0, torque.Get(1, 0));
    TEST_ASSERT_EQUAL_DOUBLE(35, torque.Get(2, 0));

    // test inputs wrong size fails
    double m32_data[3][2];
    Matrix m32(m32_data);

    bool failed = false;
    try {
        BDotController::Control(m32, b_dot, torque);
    } catch (etl::exception e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        BDotController::Control(b, m32, torque);
    } catch (etl::exception e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);

    failed = false;
    try {
        BDotController::Control(b, b_dot, m32);
    } catch (etl::exception e) {
        failed = true;
    }
    TEST_ASSERT_TRUE(failed);
}
