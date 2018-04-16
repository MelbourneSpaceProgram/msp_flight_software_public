#include <math.h>
#include <src/util/first_order_iir_lowpass.h>

FirstOrderIirLowpass::FirstOrderIirLowpass(uint16_t time_constant_millis,
                                           uint16_t sample_period_millis)
    : state(0),
      alpha(exp(-(static_cast<double>(sample_period_millis)) /
                (static_cast<double>(time_constant_millis)))) {}

double FirstOrderIirLowpass::ProcessSample(double new_sample) {
    state = alpha * state + (1 - alpha) * new_sample;
    return state;
}
