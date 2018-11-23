#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_H
#define SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_H

#include <src/database/flash_memory/flash_memory_management.h>
#include <src/database/flash_memory/flash_storables/generic_flash_storable.h>
#include <src/util/data_types.h>

template <typename FlashStorableStruct>
class FlashStorable : GenericFlashStorable {
   public:
    FlashStorable(FlashStorageAddress flash_address)
        : GenericFlashStorable(flash_address, sizeof(FlashStorableStruct)) {}

    void StoreInFlash() {
        FlashStorableStruct flash_storable_struct;
        ConvertToFlashStorableStruct(&flash_storable_struct);
        uint32_t* flash_storable_struct_byte_ptr = reinterpret_cast<uint32_t*>(
            &flash_storable_struct);  // read the struct as an array of uint32_ts
                                     // (which is fine because the compiler
                                     // packs structs in 4-byte words)
        FlashMemoryManagement::WriteToFlashMemory(
            kFlashStorageAddress, kFlashStorageSize,
            flash_storable_struct_byte_ptr);
    }

    bool UpdateFromFlash() {
        uint32_t flash_storable_struct_byte_ptr[kFlashStorageSize];
        FlashMemoryManagement::ReadFromFlashMemory(
            kFlashStorageAddress, kFlashStorageSize,
            flash_storable_struct_byte_ptr);
        // TODO (hugorilla) check if flash is valid
        // if
        // (!FlashMemoryManagement::FlashDataValid(flash_storable_struct_byte_ptr))
        // {
        //     return false;
        // }
        // or something.
        // If flash has 0xffs then it's fressssssh
        if (flash_storable_struct_byte_ptr[0] == 0xff) {
            return false;
        }
        FlashStorableStruct* flash_storable_struct =
            reinterpret_cast<FlashStorableStruct*>(
                flash_storable_struct_byte_ptr);
        UpdateFromFlashStorableStruct(flash_storable_struct);
        return true;
    }

   private:
    virtual void ConvertToFlashStorableStruct(
        FlashStorableStruct* flash_storable_struct) = 0;
    virtual void UpdateFromFlashStorableStruct(
        FlashStorableStruct* flash_storable_struct) = 0;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_H
