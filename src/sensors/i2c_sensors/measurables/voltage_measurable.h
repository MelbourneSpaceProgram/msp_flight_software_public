#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_VOLTAGE_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_VOLTAGE_MEASURABLE_H_

#include <src/messages/VoltageReading.pb.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class VoltageMeasurable : public I2cMeasurable<VoltageReading> {
   public:
    VoltageMeasurable(Adc *adc, AdcMuxMode adc_line, float scaling_factor);
    VoltageReading TakeDirectI2cReading();

   private:
    AdcMuxMode adc_line;
    float scaling_factor;
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_VOLTAGE_MEASURABLE_H_
