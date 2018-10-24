#ifndef SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA169_H_
#define SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA169_H_

#include <src/sensors/i2c_sensors/current_sensors/current_sensor.h>

class Ina169 : public CurrentSensor {
   public:
    Ina169(Adc* adc, AdcMuxMode adc_line, double rs_resistance,
           double rl_resistance);

   private:
    double ConvertVoltageToCurrent(double voltage_reading);
    static constexpr double kOutputVoltageCurrentScaleFactor = 1000.0;
    double rs_resistance;
    double rl_resistance;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA169_H_
