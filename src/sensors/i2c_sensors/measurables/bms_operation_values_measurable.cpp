#include <src/sensors/i2c_sensors/measurables/bms_operation_values_measurable.h>

BmsOperationValuesMeasurable::BmsOperationValuesMeasurable(Bms* bms)
    : I2cMeasurable<BmsOperationValuesReading>(
          bms, BmsOperationValuesReading_init_default) {}

BmsOperationValuesReading BmsOperationValuesMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsOperationValuesReading();
}
