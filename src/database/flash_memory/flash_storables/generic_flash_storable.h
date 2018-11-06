#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_GENERIC_FLASH_STORABLE_H_
#define SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_GENERIC_FLASH_STORABLE_H_

#include <src/database/flash_memory/flash_memory_management.h>
#include <src/util/data_types.h>

class GenericFlashStorable {
   public:
    GenericFlashStorable(FlashStorageAddress flash_storage_address,
                         uint32_t flash_storage_size)
        : kFlashStorageAddress(flash_storage_address),
          kFlashStorageSize(flash_storage_size) {}
    virtual void StoreInFlash() = 0;
    virtual void UpdateFromFlash() = 0;

   protected:
    const FlashStorageAddress kFlashStorageAddress;
    const uint32_t kFlashStorageSize;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_GENERIC_FLASH_STORABLE_H_
