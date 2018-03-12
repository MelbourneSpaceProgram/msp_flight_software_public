#include <src/sensors/i2c_sensors/adc_measurable.h>

AdcMeasurable::AdcMeasurable() : adc(NULL), input_line(kNotRegistered) {}

AdcMeasurable::~AdcMeasurable() {}

void AdcMeasurable::Register(Adc *adc, uint8_t input_line) {
    if (adc->inputs.at(input_line) != NULL) {
        adc->inputs.insert_at(input_line, this);
        this->adc = adc;
        this->input_line = input_line;
    }
}
