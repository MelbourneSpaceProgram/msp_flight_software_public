#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_CURRENT_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_CURRENT_MEASURABLE_H_

#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class CurrentMeasurable : public I2cMeasurable<double> {
   public:
    CurrentMeasurable(Adc *adc, AdcMuxMode adc_line, float scaling_factor, float zero_bias_point);
    double TakeI2cReading();

   private:
    float scaling_factor;
    float zero_bias_point;
    AdcMuxMode adc_line;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_CURRENT_MEASURABLE_H_
