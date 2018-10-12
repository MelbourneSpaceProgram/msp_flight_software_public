#ifndef SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_STATUS_MEASURABLE_H_
#define SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_STATUS_MEASURABLE_H_

#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/bms/runnable_bms_charge_tracker.h>
#include <src/messages/BmsChargingInfoReading.pb.h>
#include <src/sensors/i2c_sensors/measurables/i2c_measurable.h>

class Bms;

class BmsChargingInfoMeasurable : public I2cMeasurable<BmsChargingInfoReading> {
    friend class RunnableBmsChargeTracker;

   public:
    explicit BmsChargingInfoMeasurable(Bms* bms);
    BmsChargingInfoReading TakeDirectI2cReading();
};

#endif  // SRC_SENSORS_I2C_SENSORS_MEASURABLES_BMS_CHARGING_INFO_MEASURABLE_H_
