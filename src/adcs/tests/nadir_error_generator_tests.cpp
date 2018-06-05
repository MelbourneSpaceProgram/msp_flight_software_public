#include <math.h>
#include <src/adcs/state_estimators/nadir_error_generator.h>
#include <src/util/matrix.h>
#include <test_runners/unity.h>

void TestErrorQuaternionGenerator() {
    double aData[4][1] = {{0.0603}, {0.8606}, {0.2338}, {0.3798}};

    double bData[4][1] = {{-0.5312}, {-0.5863}, {-0.9253}, {0.3267}};

    double cData[4][1] = {{0}, {0}, {0}, {0}};

    Matrix a(aData), b(bData), c(cData);

    ErrorQuaternionCalculatorEarthSensor(a, b, c);

    double quat1 = c.Get(0, 0);
    double quat2 = c.Get(1, 0);
    double quat3 = c.Get(2, 0);
    double quat4 = c.Get(3, 0);

    TEST_ASSERT_DOUBLE_WITHIN(0.01, 0.4803, quat1);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, -0.3752, quat2);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 0.4419, quat3);
    TEST_ASSERT_DOUBLE_WITHIN(0.01, 0.5782, quat4);
}
