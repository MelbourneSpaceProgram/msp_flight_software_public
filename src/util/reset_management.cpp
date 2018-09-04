#include <src/util/reset_management.h>

void ResetManagement::ReadResetDataFromFlash(
    etl::array<uint32_t, kResetDataLength>& buffer) {
    uint32_t i;
    for (i = 0; i < kResetDataLength; i++) {
        buffer[i] = ((uint32_t*)kResetDataAddress)[i];
    }
}

void ResetManagement::WriteResetDataToFlash(
    etl::array<uint32_t, kResetDataLength>& new_reset_data) {
    uint32_t new_reset_data_array[kResetDataLength];
    uint32_t i;
    for (i = 0; i < kResetDataLength; i++) {
        new_reset_data_array[i] = new_reset_data[i];
    }

    // write to flash memory
    FlashProtectSet(kResetDataAddress, FlashReadWrite);
    FlashErase(kResetDataAddress);
    bool success = FlashProgram(new_reset_data_array, kResetDataAddress,
                                kResetDataLength * sizeof(uint32_t));
}

// TODO(hugorilla): split this into a flag set and a reset
void ResetManagement::ResetSystem() {
    etl::array<uint32_t, kResetDataLength> new_reset_data;
    ReadResetDataFromFlash(new_reset_data);
    new_reset_data[kResetHasOccurredIndex] = kResetHasOccurred;
    WriteResetDataToFlash(new_reset_data);
    SysCtlReset();
}

void ResetManagement::SetResetFlag(uint32_t reset_flag) {
    etl::array<uint32_t, kResetDataLength> new_reset_data;
    ReadResetDataFromFlash(new_reset_data);
    new_reset_data[kResetFlagIndex] = reset_flag;
    WriteResetDataToFlash(new_reset_data);
}
