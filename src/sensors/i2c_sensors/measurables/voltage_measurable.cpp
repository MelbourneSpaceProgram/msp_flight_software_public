#include <src/sensors/i2c_sensors/measurables/voltage_measurable.h>

VoltageMeasurable::VoltageMeasurable(Adc* adc, AdcMuxMode adc_line)
    : I2cMeasurable<float>(adc), adc_line(adc_line) {}

float VoltageMeasurable::TakeI2cReading() {
    Adc* adc = static_cast<Adc*>(I2cMeasurable::sensor);
    adc->SetMuxMode(adc_line);
    return adc->TakeI2cReading();
}
