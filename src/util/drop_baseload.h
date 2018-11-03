#ifndef SRC_UTIL_DROP_BASELOAD_H_
#define SRC_UTIL_DROP_BASELOAD_H_

#include <math.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/util/data_types.h>

class DropBaseload {
   public:
    static void Initialize();
    static void CutPowerFromPanels();
    static void RestorePowerFromPanels();
    static void CutPowerToFlightSystems();
    static void RestorePowerToFlightSystems();
    static void CutPowerToTelecoms();
    static void RestorePowerToTelecoms();

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
};

#endif  // SRC_UTIL_DROP_BASELOAD_H_
