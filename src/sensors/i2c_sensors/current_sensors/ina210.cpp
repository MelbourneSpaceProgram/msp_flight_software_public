#include <src/sensors/i2c_sensors/current_sensors/ina210.h>

Ina210::Ina210(Adc* adc, AdcMuxMode adc_line, double rs_resistance,
               double v_ref)
    : CurrentSensor(adc, adc_line),
      rs_resistance(rs_resistance),
      v_ref(v_ref) {}

double Ina210::ConvertVoltageToCurrent(double voltage_reading) {
    return (voltage_reading - v_ref) /
           (kIna210Gain * rs_resistance);  // formula from datasheet
}
