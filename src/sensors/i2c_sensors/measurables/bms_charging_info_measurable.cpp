#include <src/sensors/i2c_sensors/measurables/bms_charging_info_measurable.h>

BmsChargingInfoMeasurable::BmsChargingInfoMeasurable(Bms* bms)
    : I2cMeasurable<BmsChargingInfoReading>(
          bms, BmsChargingInfoReading_init_default) {}

BmsChargingInfoReading BmsChargingInfoMeasurable::TakeDirectI2cReading() {
    Bms* bms = static_cast<Bms*>(sensor);
    return bms->GetBmsChargingInfoReading();
}
