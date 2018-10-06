#include <src/sensors/i2c_sensors/measurables/bms_voltages_measurable.h>

BmsVoltagesMeasurable::BmsVoltagesMeasurable(Bms* bms)
    : I2cMeasurable<BmsVoltagesReading>(bms, BmsVoltagesReading_init_default) {}

BmsVoltagesReading BmsVoltagesMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsVoltagesReading();
}
