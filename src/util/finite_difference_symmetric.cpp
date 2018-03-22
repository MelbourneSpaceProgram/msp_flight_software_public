#include <src/util/finite_difference_symmetric.h>

FiniteDifferenceSymmetric::FiniteDifferenceSymmetric(
    uint16_t sample_period_millis)
    : sample_period_seconds(static_cast<double>(sample_period_millis) / 1000.0),
      sample_one_ago(0),
      sample_two_ago(0){};

double FiniteDifferenceSymmetric::ProcessSample(double new_sample) {
    double derivative =
        (new_sample - sample_two_ago) / (2 * sample_period_seconds);
    sample_two_ago = sample_one_ago;
    sample_one_ago = new_sample;
    return derivative;
}
