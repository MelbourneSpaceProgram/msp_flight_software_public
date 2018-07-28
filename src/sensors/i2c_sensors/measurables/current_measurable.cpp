#include <src/sensors/i2c_sensors/measurables/current_measurable.h>

CurrentMeasurable::CurrentMeasurable(Adc* adc, AdcMuxMode adc_line, float scaling_factor, float zero_bias_point = 0)
    : I2cMeasurable<double>(adc), adc_line(adc_line), scaling_factor(scaling_factor), zero_bias_point(zero_bias_point) {}

double CurrentMeasurable::TakeI2cReading() {
    Adc* adc = static_cast<Adc*>(I2cMeasurable::sensor);
    adc->SetMuxMode(adc_line);
    adc->SetOperationalStatus(kAdcConversion);
    return (adc->TakeI2cReading() - zero_bias_point) * scaling_factor;
}
