#include <src/sensors/i2c_sensors/measurables/bms_readings_measurable.h>

// TODO(hugorilla): work out how to put this in the .proto file
const BmsReadings BmsReadingsMeasurable::kFailedBmsReadings = {
    -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999,
    -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, 0};

BmsReadingsMeasurable::BmsReadingsMeasurable(Bms* bms)
    : I2cMeasurable<BmsReadings>(bms, kFailedBmsReadings) {}

BmsReadings BmsReadingsMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsReadings();
}
