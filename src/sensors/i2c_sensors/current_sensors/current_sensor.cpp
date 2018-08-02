#include <src/sensors/i2c_sensors/current_sensors/current_sensor.h>

CurrentSensor::CurrentSensor(Adc* adc, AdcMuxMode adc_line)
    : I2cDevice(adc->GetI2cBus(), adc->GetI2cAddress(),
                adc->GetI2cMultiplexer(), adc->GetMultiplexerChannel()),
      adc(adc),
      adc_line(adc_line) {}

double CurrentSensor::TakeCurrentReading() {
    adc->SetMuxMode(adc_line);
    double adc_voltage = adc->TakeI2cReading();
    return ConvertVoltageToCurrent(adc_voltage);
}
