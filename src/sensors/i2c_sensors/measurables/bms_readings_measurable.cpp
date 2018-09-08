#include <src/sensors/i2c_sensors/measurables/bms_readings_measurable.h>

BmsReadingsMeasurable::BmsReadingsMeasurable(Bms* bms)
    : I2cMeasurable<BmsReadings>(bms, BmsReadings_init_default) {}

BmsReadings BmsReadingsMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsReadings();
}
