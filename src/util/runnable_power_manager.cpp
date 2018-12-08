#include <src/config/satellite.h>
#include <src/sensors/measurable_id.h>
#include <src/telecomms/antenna.h>
#include <src/telecomms/lithium.h>
#include <src/util/runnable_power_manager.h>
#include <src/util/tirtos_utils.h>
#include <xdc/runtime/Log.h>

SatellitePower::BmsId RunnablePowerManager::current_bms_id =
    SatellitePower::kBmsBusC;

RunnablePowerManager::RunnablePowerManager() {}

fnptr RunnablePowerManager::GetRunnablePointer() { return &ManagePower; }

// Explicit state machine for dynamically controlling ICharge.
// Drive ICharge to the highest value where both batteries will charge,
// then hold it there for a period.
// TODO (rskew):
// - what if the satellite is spinning quickly?
void RunnablePowerManager::ManagePower() {
    IChargeState state = ICHARGE_INCREASING;
    uint64_t millis_since_hold_start = 0;
    uint8_t movement_counter = 0;
    IArg power_key;
    while (1) {
        try {
            switch (state) {
                case ICHARGE_INCREASING: {
                    movement_counter += 1;
                    // The lock makes sure that no other tasks are
                    // simultaneously writing power setting, as well as stopping
                    // this task from optimising charging during transmission of
                    // antenna burn.
                    {
                        MutexLocker locker(SatellitePower::GetMutex());
                        SatellitePower::IncrementBmsICharge(current_bms_id);
                    }
                    // Too many increments in a row suggests some kind of error.
                    // Try the other BMS.
                    if (movement_counter > kMaxSequentialMovements) {
                        ToggleBms();
                        movement_counter = 0;
                    }
                    if (!SatellitePower::BatteryIsCharging(current_bms_id)) {
                        movement_counter = 0;
                        state = ICHARGE_DECREASING;
                    }
                    break;
                }
                case ICHARGE_DECREASING: {
                    {
                        MutexLocker locker(SatellitePower::GetMutex());
                        SatellitePower::DecrementBmsICharge(current_bms_id);
                    }
                    movement_counter += 1;
                    // Too many decrements in a row indicates continuous
                    // failure to charge or read the battery current.
                    // In this case, switch to the other BMS.
                    if (movement_counter > kMaxSequentialMovements) {
                        ToggleBms();
                        movement_counter = 0;
                        state = ICHARGE_INCREASING;
                    } else if (SatellitePower::BatteryIsCharging(
                                   current_bms_id)) {
                        movement_counter = 0;
                        millis_since_hold_start = 0;
                        // Optimise the other BMS after this hold cycle.
                        ToggleBms();
                        state = ICHARGE_HOLDING;
                    }
                    break;
                }
                case ICHARGE_HOLDING: {
                    if (!SatellitePower::BatteryIsCharging(current_bms_id)) {
                        // Immediately drop ICharge if battery isn't charging
                        state = ICHARGE_DECREASING;
                    } else if (millis_since_hold_start < SystemConfiguration::GetInstance()->GetIChargeHoldTimeMs()) {
                        // ICharge is in the optimal state (as of the start of
                        // the hold), keep holding.
                        TirtosUtils::SleepMilli(SystemConfiguration::GetInstance()->GetIChargeHoldPollMs());
                        millis_since_hold_start += SystemConfiguration::GetInstance()->GetIChargeHoldPollMs();
                    } else {
                        state = ICHARGE_INCREASING;
                    }
                    break;
                }
            }
        } catch (MspException& e) {
            MspException::LogTopLevelException(e, kRunnablePowerManagerCatch);
        }
    }
}

void RunnablePowerManager::ToggleBms() {
    if (current_bms_id == SatellitePower::kBmsBusD) {
        current_bms_id = SatellitePower::kBmsBusC;
    } else {
        current_bms_id = SatellitePower::kBmsBusD;
    }
}
