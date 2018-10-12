#ifndef SRC_BOARD_I2C_BMS_RUNNABLE_BMS_CHARGE_TRACKER_H_
#define SRC_BOARD_I2C_BMS_RUNNABLE_BMS_CHARGE_TRACKER_H_

#include <src/board/i2c/bms/bms.h>
#include <src/sensors/i2c_sensors/measurables/bms_charging_info_measurable.h>
#include <src/tasks/runnable.h>
#include <src/util/data_types.h>

class BmsChargingInfoMeasurable;

class RunnableBmsChargeTracker : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnableBmsChargeTracker();

   private:
    static void TrackFirstCharge();
    static bool CheckChargeStateAndUpdate(
        BmsChargingInfoMeasurable* bms_charging_info_measurable);

    static const uint32_t kChargeStateCheckingPeriod = 5000;
};

#endif  // SRC_BOARD_I2C_BMS_RUNNABLE_BMS_CHARGE_TRACKER_H_
