#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_VOLTAGE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_VOLTAGE_MEASURABLE_H_

#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class VoltageMeasurable : public I2cMeasurable<double> {
   public:
    VoltageMeasurable(Adc *adc, AdcMuxMode adc_line, float scaling_factor);
    double TakeI2cReading();

   private:
    float scaling_factor;
    AdcMuxMode adc_line;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_VOLTAGE_MEASURABLE_H_
