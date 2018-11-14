#include <src/database/flash_memory/flash_storables/rtc_time_flash.h>

RtcTimeFlash* RtcTimeFlash::instance = NULL;

RtcTimeFlash::RtcTimeFlash()
    : FlashStorable<RtcTimeFlashStruct>(kRtcTimeFlashStorageAddress) {}

void RtcTimeFlash::ConvertToFlashStorableStruct(
    RtcTimeFlashStruct* rtc_time_flash_struct) {
    rtc_time_flash_struct->rtc_time = rtc_time;
    rtc_time_flash_struct->rtc_time_offset = rtc_time_offset;
}

void RtcTimeFlash::UpdateFromFlashStorableStruct(
    RtcTimeFlashStruct* rtc_time_flash_struct) {
    rtc_time = rtc_time_flash_struct->rtc_time;
    rtc_time_offset = rtc_time_flash_struct->rtc_time_offset;
}

RtcTimeFlash* RtcTimeFlash::GetInstance() {
    if (instance == NULL) {
        instance = new RtcTimeFlash();
    }
    return instance;
}
