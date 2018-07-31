#include <src/sensors/i2c_sensors/measurables/voltage_measurable.h>

VoltageMeasurable::VoltageMeasurable(Adc* adc, AdcMuxMode adc_line, float scaling_factor)
    : I2cMeasurable<double>(adc, kFailedVoltageReading), adc_line(adc_line), scaling_factor(scaling_factor) {}

double VoltageMeasurable::TakeDirectI2cReading() {
    Adc* adc = static_cast<Adc*>(I2cMeasurable::sensor);
    adc->SetMuxMode(adc_line);
    adc->SetOperationalStatus(kAdcConversion);
    return adc->TakeI2cReading() * scaling_factor;
}
