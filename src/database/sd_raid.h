#ifndef SRC_DATABASE_SD_RAID_H_
#define SRC_DATABASE_SD_RAID_H_

#include <src/database/sd_card.h>
#include <src/util/data_types.h>
#include <ti/sysbios/gates/GateMutexPri.h>

class SdRaid {
   public:
    enum SdStates {
        kNominal = 0,
        kFailed = 1,
        kHardFailed = 2,
    };

    // TODO(dingbenjamin): Protect on multithreaded access
    static SdRaid *GetInstance();
    bool FileCreate(const char *path);
    bool FileWrite(const char *path, const void *write_buffer,
                   uint32_t location, uint32_t num_bytes);
    bool FileRead(const char *path, void *read_buffer, uint32_t location,
                  uint32_t num_bytes);
    void FileSeek(File *f, uint32_t dest);
    void FileDelete(const char *path);
    uint32_t FileSize(File *f);
    bool Format(uint8_t index);
    bool FormatAll();

   private:
    SdRaid();
    bool IsAlive();
    void CloseGracefully(SdCard *card, SdHandle handle, File *open_file);
    static SdRaid *instance;
    static constexpr uint8_t kRaidSize = 3;
    SdCard *sd_cards[kRaidSize];
    SdStates sd_states[kRaidSize];

    GateMutexPri_Params mutex_params;
    GateMutexPri_Handle raid_mutex;

    void FileDump(File *src, char *fpath);
};
#endif  // SRC_DATABASE_SD_RAID_H_
