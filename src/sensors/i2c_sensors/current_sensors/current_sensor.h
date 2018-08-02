#ifndef SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_CURRENT_SENSOR_H_
#define SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_CURRENT_SENSOR_H_

#include <src/board/i2c/i2c.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/i2c_device.h>

class CurrentSensor : public I2cDevice {
   public:
    CurrentSensor(Adc* adc, AdcMuxMode adc_line);
    double TakeCurrentReading();
    virtual double ConvertVoltageToCurrent(double voltage_reading) = 0;

   protected:
    Adc* adc;
    AdcMuxMode adc_line;
};
#endif  //  SRC_SENSORS_I2C_SENSORS_CURRENT_SENSORS_CURRENT_SENSOR_H_
