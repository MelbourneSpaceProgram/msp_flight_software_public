#include <CppUTest/TestHarness.h>
#include <src/util/finite_difference_symmetric.h>

TEST_GROUP(FiniteDifferenceSymmetric){};

TEST(FiniteDifferenceSymmetric, TestProcessSampleDifferentiator) {
    double samples[8] = {1, 1, 2, 3, 5, 8, 13, 21};
    uint16_t sample_period_millis = 500;
    double expected_derivative[6] = {1, 2, 3, 5, 8, 13};

    FiniteDifferenceSymmetric differentiator(sample_period_millis);
    differentiator.ProcessSample(samples[0]);
    differentiator.ProcessSample(samples[1]);

    for (uint8_t i = 2; i < 8; i++) {
        double actual_derivative = differentiator.ProcessSample(samples[i]);
        DOUBLES_EQUAL(expected_derivative[i - 2], actual_derivative, 0.001);
    }
}
