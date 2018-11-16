#include <src/board/MSP432E.h>
#include <src/config/satellite.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_power.h>
#include <src/util/task_utils.h>
#include <ti/drivers/GPIO.h>
#include <xdc/runtime/Log.h>
#include <algorithm>

GateMutexPri_Params SatellitePower::mutex_params = {NULL};
GateMutexPri_Handle SatellitePower::power_mutex = NULL;
Bms* SatellitePower::bms[2];
uint8_t SatellitePower::i_charge_index[2] = {kInitialIChargeIndex,
                                             kInitialIChargeIndex};
constexpr int SatellitePower::kBmsCurrentsMeasurableId[2];

void SatellitePower::Initialize(Bms* bms_bus_d, Bms* bms_bus_c) {
    bms[kBmsBusD] = bms_bus_d;
    bms[kBmsBusC] = bms_bus_c;

    GateMutexPri_Params_init(&mutex_params);
    power_mutex = GateMutexPri_create(&mutex_params, NULL);
    if (power_mutex == NULL) {
        throw etl::exception("Failed to create SatellitePower mutex", __FILE__,
                             __LINE__);
    }

    IArg key = Lock();
    CutPowerToTelecoms();

    const IoExpander* io_expander_bms = IoExpander::GetIoExpander(0);

    try {
        io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
        io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
        io_expander_bms->SetPin(kIoExpanderPinFSEn, false);

        io_expander_bms->SetDirection(kIoExpanderPinBms1En,
                                      IoExpander::kIoOutput);
        io_expander_bms->SetDirection(kIoExpanderPinBms2En,
                                      IoExpander::kIoOutput);
        io_expander_bms->SetPolarity(kIoExpanderPinBms1En,
                                     IoExpander::kIoActiveHigh);
        io_expander_bms->SetPolarity(kIoExpanderPinBms2En,
                                     IoExpander::kIoActiveHigh);

        io_expander_bms->SetDirection(kIoExpanderPinFSEn,
                                      IoExpander::kIoOutput);
        io_expander_bms->SetPolarity(kIoExpanderPinFSEn,
                                     IoExpander::kIoActiveHigh);
    } catch (etl::exception& e) {
        MspException::LogException(e);
        Log_error0("BMS IO expander failed to initialise properly");
    }
    Unlock(key);
}

IArg SatellitePower::Lock() { return GateMutexPri_enter(power_mutex); }

void SatellitePower::Unlock(IArg key) {
    return GateMutexPri_leave(power_mutex, key);
}

void SatellitePower::CutPowerFromPanels() {
    Log_info0("Cutting power from solar panels");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, false);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, false);
}

void SatellitePower::RestorePowerFromPanels() {
    Log_info0("Restoring power from solar panels");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
}

void SatellitePower::CutPowerToFlightSystems() {
    Log_info0("Cutting power to Flight Systems");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, false);
}

void SatellitePower::RestorePowerToFlightSystems() {
    Log_info0("Restoring power to Flight Systems");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, true);
}

void SatellitePower::CutPowerToTelecoms() {
    Log_info0("Cutting power to Telecoms");
    GPIO_write(nCOMMS_RST, 0);
}

void SatellitePower::RestorePowerToTelecoms() {
    Log_info0("Restoring power to Telecoms");
    GPIO_write(nCOMMS_RST, 1);
}

bool SatellitePower::ConfigureBms(BmsId bms_id) {
    return bms[bms_id]->SetConfiguration(i_charge_index[bms_id]);
}

bool SatellitePower::ConfigureBmsICharge(BmsId bms_id) {
    Log_info2("Re-configuring BMS bus %d with I-charge index: %d", bms_id,
              i_charge_index[bms_id]);
    bool success = bms[bms_id]->SetICharge(i_charge_index[bms_id]);
    return success;
}

Bms* SatellitePower::GetBms(BmsId bms_id) { return bms[bms_id]; }

void SatellitePower::IncrementBmsICharge(BmsId bms_id) {
    uint8_t incremented_index = i_charge_index[bms_id] + 1;
    if (incremented_index < Bms::kIChargeIndexMax) {
        i_charge_index[bms_id] = incremented_index;
    } else {
        i_charge_index[bms_id] = Bms::kIChargeIndexMax - 1;
    }
    ConfigureBmsICharge(bms_id);
    TaskUtils::SleepMilli(Bms::kBmsTryChargeDecreaseWaitMs);
}

void SatellitePower::DecrementBmsICharge(BmsId bms_id) {
    i_charge_index[bms_id] = std::max(i_charge_index[bms_id] - 1, 0);
    ConfigureBmsICharge(bms_id);
    TaskUtils::SleepMilli(Bms::kBmsTryChargeDecreaseWaitMs);
}

bool SatellitePower::BatteryIsCharging(BmsId bms_id) {
    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();
    BmsCurrentsReading bms_currents =
        measurable_manager->ReadNanopbMeasurable<BmsCurrentsReading>(
            kBmsCurrentsMeasurableId[bms_id], 0);
    // Check value is in a valid range
    if (bms_currents.battery_current < Bms::kBmsMinimumValidCurrentReadingA ||
        bms_currents.battery_current > Bms::kBmsMaximumValidCurrentReadingA) {
        Log_info2("Invalid battery current reading from BMS %d of: %f", bms_id,
                  bms_currents.battery_current);
        return false;
    }
    if (bms_currents.battery_current ==
        measurable_manager
            ->GetMeasurable<BmsCurrentsReading>(
                kBmsCurrentsMeasurableId[bms_id])
            ->GetFailureReading()
            .battery_current) {
        return false;
    }
    return bms_currents.battery_current >
           Bms::kBatteryChargeCurrentLowerBoundA;
}

uint8_t SatellitePower::GetIChargeIndex(BmsId bms_id) {
    return i_charge_index[bms_id];
}
