#ifndef SRC_UTIL_SATELLITE_POWER_H_
#define SRC_UTIL_SATELLITE_POWER_H_

#include <math.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/sensors/measurable_id.h>
#include <src/util/data_types.h>
#include <ti/sysbios/gates/GateMutexPri.h>

class SatellitePower {
   public:
    enum BmsId {
        kBmsBusD = 0,
        kBmsBusC = 1,
        kNumBms,
    };
    static void Initialize(Bms* bms1, Bms* bms2);
    static void CutPowerFromPanels();
    static void RestorePowerFromPanels();
    static void CutPowerToFlightSystems();
    static void RestorePowerToFlightSystems();
    static void CutPowerToTelecoms();
    static void RestorePowerToTelecoms();
    static Bms* GetBms(BmsId bms_id);
    static void IncrementBmsICharge(BmsId bms_id);
    static void DecrementBmsICharge(BmsId bms_id);
    static bool BatteryIsCharging(BmsId bms_id);
    static uint8_t GetIChargeIndex(BmsId bms_id);
    static bool ConfigureBms(BmsId bms_id);
    static bool ConfigureBmsICharge(BmsId bms_id);
    static GateMutexPri_Handle& GetMutex();

    static constexpr IoExpander::IoPin kIoExpanderPinBms1En =
        IoExpander::kIoPin0;
    static constexpr IoExpander::IoPin kIoExpanderPinBms2En =
        IoExpander::kIoPin2;
    static constexpr IoExpander::IoPin kIoExpanderPinFSEn =
        IoExpander::kIoPin15;
    static constexpr IoExpander::IoPin kIoExpanderPinTelecomsEn1 =
        IoExpander::kIoPin1;
    static constexpr IoExpander::IoPin kIoExpanderPinTelecomsEn4 =
        IoExpander::kIoPin4;

   private:
    static GateMutexPri_Params mutex_params;
    static GateMutexPri_Handle power_mutex;
	static bool initialised;
    static Bms* bms[2];
    static uint8_t i_charge_index[2];

    static constexpr int kBmsCurrentsMeasurableId[2] = {
        kEpsBmsCurrentsReading1, kEpsBmsCurrentsReading2};
};

#endif  // SRC_UTIL_SATELLITE_POWER_H_
