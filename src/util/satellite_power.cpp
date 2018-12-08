#include <assert.h>
#include <src/board/MSP432E.h>
#include <src/config/satellite.h>
#include <src/sensors/i2c_sensors/measurables/imu_magnetometer_measurable.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/msp_exception.h>
#include <src/util/satellite_power.h>
#include <src/util/tirtos_utils.h>
#include <ti/drivers/GPIO.h>
#include <xdc/runtime/Log.h>
#include <algorithm>

GateMutexPri_Params SatellitePower::mutex_params;
GateMutexPri_Handle SatellitePower::power_mutex = NULL;
bool SatellitePower::initialised = false;
Bms* SatellitePower::bms[2];
uint8_t SatellitePower::i_charge_index[2] = {kInitialIChargeIndex,
                                             kInitialIChargeIndex};
constexpr int SatellitePower::kBmsCurrentsMeasurableId[2];

void SatellitePower::Initialize(Bms* bms_bus_d, Bms* bms_bus_c) {
    assert(!initialised);
    bms[kBmsBusD] = bms_bus_d;
    bms[kBmsBusC] = bms_bus_c;

    GateMutexPri_Params_init(&mutex_params);
    power_mutex = GateMutexPri_create(&mutex_params, NULL);
    if (power_mutex == NULL) {
        throw MspException("Failed to create SatellitePower mutex",
                           kSatellitePowerMutexFail, __FILE__, __LINE__);
    }

    MutexLocker locker(power_mutex);
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

        io_expander_bms->SetPin(kIoExpanderPinMuxCReset, false);
        io_expander_bms->SetPolarity(kIoExpanderPinMuxCReset,
                                     IoExpander::kIoActiveLow);
        io_expander_bms->SetDirection(kIoExpanderPinMuxCReset,
                                      IoExpander::kIoOutput);
    } catch (MspException& e) {
        MspException::LogException(e, kSatellitePowerInitCatch);
        Log_error0("BMS IO expander failed to initialise properly");
    }
    initialised = true;
}

GateMutexPri_Handle& SatellitePower::GetMutex() { return power_mutex; }

void SatellitePower::CutPowerFromPanels() {
    if (!initialised) return;
    if (kVerboseLogging) Log_info0("Cutting power from solar panels");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, false);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, false);
}

void SatellitePower::RestorePowerFromPanels() {
    if (!initialised) return;
    if (kVerboseLogging) Log_info0("Restoring power from solar panels");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinBms1En, true);
    io_expander_bms->SetPin(kIoExpanderPinBms2En, true);
}

void SatellitePower::CutPowerToFlightSystems() {
    if (!initialised) return;
    if (kVerboseLogging) Log_info0("Cutting power to Flight Systems");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, false);
}

void SatellitePower::RestorePowerToFlightSystems() {
    if (!initialised) return;
    if (kVerboseLogging) Log_info0("Restoring power to Flight Systems");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    io_expander_bms->SetPin(kIoExpanderPinFSEn, true);

    // Give the Flight Systems board time to wake up
    TirtosUtils::SleepMilli(10);

    MeasurableManager* measurable_manager = MeasurableManager::GetInstance();

    try {
        ImuMagnetometerMeasurable* imu =
            dynamic_cast<ImuMagnetometerMeasurable*>(
                measurable_manager->GetMeasurable<MagnetometerReading>(
                    kFsImuMagno1));
        if (imu != nullptr) {
            imu->InitialiseImu();
        } else {
            throw MspException("Imu 1 was not properly initialised", kImuNull1,
                               __FILE__, __LINE__);
        }
    } catch (MspException& e) {
        MspException::LogException(e, kImuInitialise1Catch);
    }

    try {
        ImuMagnetometerMeasurable* imu =
            dynamic_cast<ImuMagnetometerMeasurable*>(
                measurable_manager->GetMeasurable<MagnetometerReading>(
                    kFsImuMagno2));
        if (imu != nullptr) {
            imu->InitialiseImu();
        } else {
            throw MspException("Imu 2 was not properly initialised", kImuNull2,
                               __FILE__, __LINE__);
        }
    } catch (MspException& e) {
        MspException::LogException(e, kImuInitialise2Catch);
    }
}

void SatellitePower::CutPowerToTelecoms() {
    if (!initialised) return;
    if (kVerboseLogging) Log_info0("Cutting power to Telecoms");
    GPIO_write(nCOMMS_RST, 0);
}

void SatellitePower::RestorePowerToTelecoms() {
    if (!initialised) return;
    if (kVerboseLogging) Log_info0("Restoring power to Telecoms");
    GPIO_write(nCOMMS_RST, 1);
}

bool SatellitePower::ConfigureBms(BmsId bms_id) {
    if (!initialised) return false;
    return bms[bms_id]->SetConfiguration(i_charge_index[bms_id]);
}

bool SatellitePower::ConfigureBmsICharge(BmsId bms_id) {
    if (!initialised) return false;
    if (kVerboseLogging)
        Log_info2("Re-configuring BMS bus %d with I-charge index: %d", bms_id,
                  i_charge_index[bms_id]);
    bool success = bms[bms_id]->SetICharge(i_charge_index[bms_id]);
    return success;
}

Bms* SatellitePower::GetBms(BmsId bms_id) { return bms[bms_id]; }

void SatellitePower::IncrementBmsICharge(BmsId bms_id) {
    if (!initialised) return;
    uint8_t incremented_index = i_charge_index[bms_id] + 1;
    if (incremented_index < Bms::kIChargeIndexMax) {
        i_charge_index[bms_id] = incremented_index;
    } else {
        i_charge_index[bms_id] = Bms::kIChargeIndexMax - 1;
    }
    {
        MutexLocker locker(SatellitePower::GetMutex());
        ConfigureBmsICharge(bms_id);
    }
    TirtosUtils::SleepMilli(Bms::kBmsTryChargeDecreaseWaitMs);
}

void SatellitePower::DecrementBmsICharge(BmsId bms_id) {
    if (!initialised) return;
    i_charge_index[bms_id] = std::max(i_charge_index[bms_id] - 1, 0);
    {
        MutexLocker locker(SatellitePower::GetMutex());
        ConfigureBmsICharge(bms_id);
    }
    TirtosUtils::SleepMilli(Bms::kBmsTryChargeDecreaseWaitMs);
}

bool SatellitePower::BatteryIsCharging(BmsId bms_id) {
    if (!initialised) return false;
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
    return bms_currents.battery_current > Bms::kBatteryChargeCurrentLowerBoundA;
}

uint8_t SatellitePower::GetIChargeIndex(BmsId bms_id) {
    if (!initialised) return kInitialIChargeIndex;
    return i_charge_index[bms_id];
}

bool SatellitePower::ResetMuxC() {
    if (kVerboseLogging) Log_info0("Reset Mux C");
    const IoExpander* io_expander_bms =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    try {
        io_expander_bms->SetPin(kIoExpanderPinMuxCReset, true);
        TirtosUtils::SleepMilli(kMuxResetWaitMs);
        io_expander_bms->SetPin(kIoExpanderPinMuxCReset, false);
    } catch (MspException& e) {
        MspException::LogException(e, kIoExpanderMuxCResetCatch);
        Log_error0("Failed to reset MUX C via the EPS IO expander");
        return false;
    }
    return true;
}

void SatellitePower::ResetMuxA() {
    if (kVerboseLogging) Log_info0("Reset Mux A");
    GPIO_write(I2C_MUX_nRST, 0);
    TirtosUtils::SleepMilli(kMuxResetWaitMs);
    GPIO_write(I2C_MUX_nRST, 1);
}
