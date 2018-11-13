#include <src/database/flash_memory/flash_memory_management.h>
#include <ti/devices/msp432e4/driverlib/flash.h>
#include <xdc/runtime/Log.h>

bool FlashMemoryManagement::WriteToFlashMemory(
    FlashStorageAddress flash_storage_address,
    uint32_t flash_storage_size_bytes, uint32_t *array_to_store_in_flash) {
    // read current values into an array
    uint32_t acrux_1_data[kTotalAcrux1DataStorageSize];
    ReadFromFlashMemory(kAcrux1DataFlashAddress, kTotalAcrux1DataStorageSize,
                        acrux_1_data);

    // update array with new values to be written
    for (uint32_t i = 0; i < flash_storage_size_bytes / 4; i++) {
        acrux_1_data[flash_storage_address - kAcrux1DataFlashAddress + i] =
            array_to_store_in_flash[i];
    }

    int32_t flash_write_success =
        0;  // 0 indicates a successful flash memory interaction

    // unprotect sector
    flash_write_success +=
        FlashProtectSet(kAcrux1DataFlashBlockStartAddress, FlashReadWrite);

    // erase sector
    flash_write_success += FlashErase(kAcrux1DataFlashBlockStartAddress);

    // read array values back into the flash memory
    flash_write_success += FlashProgram(acrux_1_data, kAcrux1DataFlashAddress,
                                        kTotalAcrux1DataStorageSizeBytes);

    // TODO(hugorilla): investigate the consequences of leaving the flash memory
    // in 'FlashReadWrite' protection.
    // Previously the sector unprotected above was re-protected after the flash
    // program operation, but this was causing errors when attempting to
    // re-program the board ("Flash Programmer: Mass erase failed"). Most
    // documentation online suggests that protecting the sector after writing to
    // it is recommended, so more research is needed to justify removing it
    // here.
    return (flash_write_success == 0);
}

void FlashMemoryManagement::ReadFromFlashMemory(
    FlashStorageAddress flash_storage_address,
    uint32_t flash_storage_size_bytes, uint32_t *array_retrieved_from_flash) {
    uint32_t *flash_address_ptr =
        reinterpret_cast<uint32_t *>(flash_storage_address);
    for (uint32_t i = 0; i < flash_storage_size_bytes / 4; i++) {
        array_retrieved_from_flash[i] = flash_address_ptr[i];
    }
}

void FlashMemoryManagement::EraseFlashMemory() {
    if (!FlashErase(kAcrux1DataFlashBlockStartAddress)) {
        Log_error0("Flash erase was not successful");
    }
}
