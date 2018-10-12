#include <src/board/i2c/bms/runnable_bms_charge_tracker.h>
#include <src/messages/BmsChargingInfoReading.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/task_utils.h>

RunnableBmsChargeTracker::RunnableBmsChargeTracker() {}

fnptr RunnableBmsChargeTracker::GetRunnablePointer() {
    return &RunnableBmsChargeTracker::TrackFirstCharge;
}

void RunnableBmsChargeTracker::TrackFirstCharge() {
    MeasurableManager* measurable_manager =
        MeasurableManager::GetInstance();

    BmsChargingInfoMeasurable* bms_1_charging_info_measurable =
        static_cast<BmsChargingInfoMeasurable*>(
            measurable_manager->GetMeasurable<BmsChargingInfoReading>(
                kEpsBmsChargingInfoReading1));
    BmsChargingInfoMeasurable* bms_2_charging_info_measurable =
        static_cast<BmsChargingInfoMeasurable*>(
            measurable_manager->GetMeasurable<BmsChargingInfoReading>(
                kEpsBmsChargingInfoReading2));
    bool bms_1_first_charge_complete = false;
    bool bms_2_first_charge_complete = false;

    // loop until both BMSs have completed their first full charge
    while (!bms_1_first_charge_complete && !bms_2_first_charge_complete) {
        bms_1_first_charge_complete =
            CheckChargeStateAndUpdate(bms_1_charging_info_measurable);
        bms_2_first_charge_complete =
            CheckChargeStateAndUpdate(bms_2_charging_info_measurable);
        TaskUtils::SleepMilli(kChargeStateCheckingPeriod);
    }
}

bool RunnableBmsChargeTracker::CheckChargeStateAndUpdate(
    BmsChargingInfoMeasurable* bms_charging_info_measurable) {
    Bms* bms = static_cast<Bms*>(bms_charging_info_measurable->sensor);
    if (!bms->first_charge_complete) {
        if (bms_charging_info_measurable->TakeReading()) {
            BmsChargingInfoReading bms_charging_info_reading =
                bms_charging_info_measurable->GetReading();
            if (bms_charging_info_reading.charger_state ==
                BmsChargingInfoReading_ChargerState_kCOverXTerm) {
                bms->first_charge_complete = true;
                bms->WriteToRegister(Bms::kQCountRegisterLocation,
                                     Bms::kQCountRegisterFullChargeLBValue,
                                     Bms::kQCountRegisterFullChargeUBValue);
            }
        }
    }
    return bms->first_charge_complete;
}
