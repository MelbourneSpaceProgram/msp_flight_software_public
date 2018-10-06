#include <src/sensors/i2c_sensors/measurables/bms_currents_measurable.h>

BmsCurrentsMeasurable::BmsCurrentsMeasurable(Bms* bms)
    : I2cMeasurable<BmsCurrentsReading>(bms, BmsCurrentsReading_init_default) {}

BmsCurrentsReading BmsCurrentsMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsCurrentsReading();
}
