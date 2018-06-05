#include <math.h>
#include <src/util/first_order_iir_lowpass.h>
#include <test_runners/unity.h>

void TestProcessSampleFilter() {
    double test_input[5] = {1, 1, 1, 1, 1};
    uint16_t time_constant_millis = 1000;
    uint16_t sample_period_millis = 50;
    double alpha = exp(-static_cast<double>(sample_period_millis) /
                       static_cast<double>(time_constant_millis));
    double expected_output[5] = {0.048770575499286, 0.095162581964040,
                                 0.139292023574942, 0.181269246922018,
                                 0.221199216928595};

    FirstOrderIirLowpass filter(time_constant_millis, sample_period_millis);

    for (uint8_t i = 0; i < 5; i++) {
        double filter_output = filter.ProcessSample(test_input[i]);
        TEST_ASSERT_EQUAL_DOUBLE(expected_output[i], filter_output);
    }
}
