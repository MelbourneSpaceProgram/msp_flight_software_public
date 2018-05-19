#ifndef SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA212_H_
#define SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA212_H_

#include <src/sensors/i2c_sensors/current_sensors/current_sensor.h>

class Ina212 : public CurrentSensor {
   public:
    Ina212(Adc* adc, AdcMuxMode adc_line, double rs_resistance,
           double v_ref);

   private:
    double ConvertVoltageToCurrent(double voltage_reading);
    static const double kIna212Gain = 1000;
    double rs_resistance;
    double v_ref;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA212_H_
