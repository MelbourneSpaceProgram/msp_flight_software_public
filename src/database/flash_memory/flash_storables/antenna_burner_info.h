#ifndef SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_ANTENNA_BURNER_INFO_H_
#define SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_ANTENNA_BURNER_INFO_H_

#include <src/database/flash_memory/flash_storables/flash_storable.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>

class AntennaBurnerInfo : public FlashStorable<AntennaBurnerInfoStruct> {
    friend class RunnableAntennaBurner;
    friend class TerminateAntennaBurnUplink;

   public:
    AntennaBurnerInfo();
    uint64_t GetLastBurnAttempt();
    uint64_t GetBurnInterval();
    bool GetAttemptBurnSetting();
    void SetLastBurnAttempt(uint64_t new_burn_attempt_timestamp_ms);
    void SetBurnInterval(uint64_t new_burn_interval_ms);
    void SetAntennaBurnSetting(bool attempt_burn);

   private:
    void ConvertToFlashStorableStruct(
        AntennaBurnerInfoStruct *antenna_burner_info_struct);
    void UpdateFromFlashStorableStruct(
        AntennaBurnerInfoStruct *antenna_burner_info_struct);

    uint64_t last_burn_attempt_timestamp_ms;
    uint64_t burn_interval_ms;
    bool attempt_antenna_burn;
};

#endif  // SRC_DATABASE_FLASH_MEMORY_FLASH_STORABLES_ANTENNA_BURNER_INFO_H_
