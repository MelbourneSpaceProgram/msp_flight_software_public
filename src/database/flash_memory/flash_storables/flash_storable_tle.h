#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_TLE_H_
#define SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_TLE_H_

#include <src/database/flash_memory/flash_storables/flash_storable.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>
#include <src/messages/Tle.pb.h>

class FlashStorableTle : public FlashStorable<FlashStorableTleStruct> {
   public:
    FlashStorableTle(Tle& tle);

   private:
    void ConvertToFlashStorableStruct(
        FlashStorableTleStruct* flash_storable_tle_struct);
    void UpdateFromFlashStorableStruct(
        FlashStorableTleStruct* flash_storable_tle_struct);

    FlashStorableTleStruct tle_data;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_FLASH_STORABLE_TLE_H_
