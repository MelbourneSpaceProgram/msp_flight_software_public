#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/voltage_measurable.h>

VoltageMeasurable::VoltageMeasurable(Adc* adc, AdcMuxMode adc_line,
                                     float scaling_factor)
    : I2cMeasurable<VoltageReading>(adc, VoltageReading_init_default),
      adc_line(adc_line),
      scaling_factor(scaling_factor) {}

VoltageReading VoltageMeasurable::TakeDirectI2cReading() {
    Adc* adc = static_cast<Adc*>(I2cMeasurable::sensor);
    adc->SetMuxMode(adc_line);
    adc->SetOperationalStatus(kAdcConversion);
    VoltageReading reading = VoltageReading_init_default;
    reading.voltage = adc->TakeI2cReading() * scaling_factor;
    return reading;
}
