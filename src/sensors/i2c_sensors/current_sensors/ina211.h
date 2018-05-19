#ifndef SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA211_H_
#define SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA211_H_

#include <src/sensors/i2c_sensors/current_sensors/current_sensor.h>

class Ina211 : public CurrentSensor {
   public:
    Ina211(Adc* adc, AdcMuxMode adc_line, double rs_resistance,
           double v_ref);

   private:
    double ConvertVoltageToCurrent(double voltage_reading);
    static const double kIna211Gain = 500;
    double rs_resistance;
    double v_ref;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_INA211_H_
