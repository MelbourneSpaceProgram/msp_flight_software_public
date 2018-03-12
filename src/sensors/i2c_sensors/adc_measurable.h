#ifndef SRC_I2C_ADC_ADC_MEASURABLE_H_
#define SRC_I2C_ADC_ADC_MEASURABLE_H_

#include <src/sensors/i2c_sensors/adc.h>
#include <src/util/data_types.h>

class Adc;

class AdcMeasurable {
   public:
    static const uint8_t kNotRegistered = 255;
    virtual ~AdcMeasurable();

   protected:
    AdcMeasurable();
    void Register(Adc *adc, uint8_t input_line);

   private:
    Adc *adc;
    uint8_t input_line;
};

#endif  // SRC_I2C_ADC_ADC_MEASURABLE_H_
