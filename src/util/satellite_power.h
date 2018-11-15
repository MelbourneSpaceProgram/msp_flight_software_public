#ifndef SRC_UTIL_SATELLITE_POWER_H_
#define SRC_UTIL_SATELLITE_POWER_H_

#include <math.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/util/data_types.h>

class SatellitePower {
   public:
    static void Initialize(Bms* bms1, Bms* bms2);
    static void CutPowerFromPanels();
    static void RestorePowerFromPanels();
    static void CutPowerToFlightSystems();
    static void RestorePowerToFlightSystems();
    static void CutPowerToTelecoms();
    static void RestorePowerToTelecoms();
    static bool ConfigureBmsBusD();
    static bool ConfigureBmsBusC();
    static Bms* GetBmsBusD();
    static Bms* GetBmsBusC();

   private:
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
    static Bms* bms_d;
    static Bms* bms_c;
};

#endif  // SRC_UTIL_SATELLITE_POWER_H_
