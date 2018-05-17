#ifndef SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_CURRENT_SENSOR_H_
#define SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_CURRENT_SENSOR_H_

#include <src/i2c/i2c.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/i2c_sensor.h>

class CurrentSensor : public I2cSensor {
   public:
    CurrentSensor(Adc* adc, AdcMuxMode adc_line);
    double TakeCurrentReading();
    virtual double ConvertVoltageToCurrent(double voltage_reading) = 0;

   protected:
    Adc* adc;
    AdcMuxMode adc_line;
};
#endif  //  SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_CURRENT_SENSOR_H_
