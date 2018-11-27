#ifndef SRC_DATABASE_FLAsH_MEMORY_FLASH_STORABLES_RESET_INFO_CONTAINER_H
#define SRC_DATABASE_FLAsH_MEMORY_FLASH_STORABLES_RESET_INFO_CONTAINER_H

#include <external/etl/map.h>
#include <math.h>
#include <src/database/flash_memory/flash_memory_management.h>
#include <src/database/flash_memory/flash_storables/flash_storable_data_types.h>
#include <src/database/flash_memory/flash_storables/flash_storable.h>
#include <ti/devices/msp432e4/driverlib/sysctl.h>

typedef etl::map<ResetCauses, uint32_t, 9, std::less<ResetCauses>>
    ResetCausesMap;

enum SatelliteOperatingModes { kNormalOperatingMode, kLimpMode };

class ResetInfoContainer : public FlashStorable<ResetInfoContainerStruct> {
    friend class ResetManagement;

   public:
    ResetInfoContainer();
    void Init();
    static ResetInfoContainer* GetInstance();
    void InitialiseValues();

    /* get methods */
    ResetCauses GetMostRecentResetCause() const;
    ResetMessage GetMostRecentResetMessage() const;
    uint32_t GetNumResets() const;
    uint32_t GetNumResetsOfCause(ResetCauses reset_type) const;
    uint32_t GetNumResetsWithMessage(ResetMessage reset_message) const;

   private:
    ResetCauses most_recent_reset_cause;
    ResetMessage most_recent_reset_message;
    uint32_t num_resets;
    uint32_t num_resets_of_cause[kNumResetCauses];
    uint32_t num_resets_with_message[kNumResetMessages];
    uint32_t first_wakeup;  // for internal use only
    bool expected_reset; // for internal use only
    // TODO(hugorilla): Store some sort of timing information in here (for
    // checking the frequency of the resets)

    void ConvertToFlashStorableStruct(
        ResetInfoContainerStruct* reset_info_container_struct);
    void UpdateFromFlashStorableStruct(
        ResetInfoContainerStruct* reset_info_container_struct);

    static ResetInfoContainer* instance;

    ResetCausesMap reset_causes_map;

    ResetCauses GetMostRecentResetCauseFromFlash();
    bool IsValidResetCause(uint32_t reset_cause);

    const static uint32_t kNotFirstWakeup = 77;  // arbitrary number
};

#endif  // SRC_DATABASE_FLAsH_MEMORY_FLASH_STORABLES_RESET_INFO_CONTAINER_H
