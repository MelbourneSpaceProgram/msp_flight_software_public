#ifndef SRC_DATABASE_TEMP_FOLDER_RTC_TIME_FLASH_H_
#define SRC_DATABASE_TEMP_FOLDER_RTC_TIME_FLASH_H_

#include <src/database/flash_memory/flash_storables/flash_storable.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>

class RtcTimeFlash : public FlashStorable<RtcTimeFlashStruct> {
   public:
    RtcTimeFlash();
    uint64_t rtc_time;
    uint64_t rtc_time_offset;
    static RtcTimeFlash* GetInstance();

   private:
    static RtcTimeFlash* instance;
    void ConvertToFlashStorableStruct(
        RtcTimeFlashStruct* rtc_time_flash_struct);
    void UpdateFromFlashStorableStruct(
        RtcTimeFlashStruct* rtc_time_flash_struct);
};

#endif  // SRC_DATABASE_TEMP_FOLDER_RTC_TIME_FLASH_H_
