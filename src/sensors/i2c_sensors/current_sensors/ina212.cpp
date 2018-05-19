#include <src/sensors/i2c_sensors/current_sensors/ina212.h>

Ina212::Ina212(Adc* adc, AdcMuxMode adc_line, double rs_resistance,
               double v_ref)
    : CurrentSensor(adc, adc_line),
      rs_resistance(rs_resistance),
      v_ref(v_ref) {}

double Ina212::ConvertVoltageToCurrent(double voltage_reading) {
    return (voltage_reading - v_ref) /
           (kIna212Gain * rs_resistance);  // formula from datasheet
}
