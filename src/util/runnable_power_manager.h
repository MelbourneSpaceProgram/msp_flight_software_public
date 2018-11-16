#ifndef SRC_UTIL_RUNNABLE_POWER_MANAGER_H_
#define SRC_UTIL_RUNNABLE_POWER_MANAGER_H_

#include <src/tasks/runnable.h>
#include <src/util/satellite_power.h>

class RunnablePowerManager : public Runnable {
   public:
    fnptr GetRunnablePointer();
    RunnablePowerManager();

   private:
    static SatellitePower::BmsId current_bms_id;
    static constexpr uint8_t kMaxSequentialMovements = 5;

    enum IChargeState {
        ICHARGE_INCREASING = 0,
        ICHARGE_DECREASING = 1,
        ICHARGE_HOLDING = 2,
    };
    static void ManagePower();
    static void ToggleBms();
};

#endif  // SRC_UTIL_RUNNABLE_POWER_MANAGER_H_
