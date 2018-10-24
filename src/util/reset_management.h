#ifndef SRC_UTIL_RESET_MANAGEMENT_H_
#define SRC_UTIL_RESET_MANAGEMENT_H_

#include <external/etl/array.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>

class ResetManagement {
   public:
    static constexpr uint32_t kResetDataLength = 2;
    static constexpr uint32_t kResetHasOccurred = 77;  // arbitrary value

    static void ReadResetDataFromFlash(
        etl::array<uint32_t, kResetDataLength>& buffer);
    static bool WriteResetDataToFlash(
        etl::array<uint32_t, kResetDataLength>& new_reset_data);
    static void ResetSystem();
    static void SetResetFlag(uint32_t reset_flag);

    // reset 'messages'
    static constexpr uint32_t kUnitTestsComplete = 1;
    static constexpr uint32_t kOrbitComplete = 2;
    static constexpr uint32_t kForceResetCommand = 3;

    // reset data array indexes
    static constexpr uint32_t kResetHasOccurredIndex = 0;
    static constexpr uint32_t kResetFlagIndex = 1;

   private:
    static constexpr uint32_t kResetDataAddress =
        0xf0000;  // specified by memory section partitioning in linker.cmd
};

#endif  // RESET_MANAGEMENT_H_
