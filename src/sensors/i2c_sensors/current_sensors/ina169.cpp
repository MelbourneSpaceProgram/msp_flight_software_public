#include <src/sensors/i2c_sensors/current_sensors/ina169.h>

Ina169::Ina169(Adc* adc, AdcMuxMode adc_line, double rs_resistance,
               double rl_resistance)
    : CurrentSensor(adc, adc_line),
      rs_resistance(rs_resistance),
      rl_resistance(rl_resistance) {}

double Ina169::ConvertVoltageToCurrent(double voltage_reading) {
    return voltage_reading * kOutputVoltageCurrentScaleFactor /
           (rs_resistance * rl_resistance);  // formula from datasheet
}
