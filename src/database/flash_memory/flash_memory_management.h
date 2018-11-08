#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_MEMORY_MANAGEMENT_H
#define SRC_DATABASE_FLASH_MEMORY_FLASH_MEMORY_MANAGEMENT_H

#include <external/etl/array.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>
#include <src/util/data_types.h>

// hard-coded memory addresses for flash storable object structs
enum FlashStorageAddress {
    kAcrux1DataFlashAddress = 0x000F0000,  // this value must match the
                                           // ACRUX1DATA memory address defined
                                           // in linker.cmd
    kResetInfoContainerFlashStorageAddress = 0x000F0000,
    kFlashStorableTleFlashStorageAddress =
        kResetInfoContainerFlashStorageAddress +
        sizeof(ResetInfoContainerStruct),
};

class FlashMemoryManagement {
   public:
    static bool WriteToFlashMemory(
        FlashStorageAddress flash_storage_address,
        uint32_t flash_storage_size_bytes,
        uint32_t *array_to_store_in_flash);  // flash_storage_size_bytes must be
                                             // a multiple of 4
    static void ReadFromFlashMemory(
        FlashStorageAddress flash_storage_address,
        uint32_t flash_storage_size_bytes,
        uint32_t *array_retrieved_from_flash);  // flash_storage_size_bytes must
                                                // be a multiple of 4

    static const uint32_t kDefaultFlashMemoryWord = 0xFFFFFFFF;  // when the
                                                                 // flash
                                                                 // memory
                                                                 // is
                                                                 // erased,
                                                                 // all bits
                                                                 // are set
                                                                 // to 1

   private:
    static const uint32_t kAcrux1DataFlashBlockStartAddress =
        0x000F0000;  // this value corresponds to the start of the block
                     // containing the kAcrux1DataFlashAddress (above in enum),
                     // and has the same value as it assuming that data is
                     // written from the beginning of the block
    static const uint32_t kAcrux1DataFlashSize =
        0x00004000;  // this value must match the ACRUX1DATA memory address
                     // defined in linker.cmd

    static constexpr uint32_t kTotalAcrux1DataStorageSizeBytes =
        sizeof(ResetInfoContainerStruct);  // sum of the sizes (in bytes) of all
                                           // flash storable object structs to
                                           // be stored in the flash memory
    static constexpr uint32_t kTotalAcrux1DataStorageSize =
        kTotalAcrux1DataStorageSizeBytes / 4;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_MEMORY_MANAGEMENT_H
