#ifndef SRC_DATABASE_SDCARD_H_
#define SRC_DATABASE_SDCARD_H_

#include <src/util/data_types.h>
#include <ti/drivers/SD.h>
#include <ti/drivers/SDFatFS.h>
#include <ti/sysbios/gates/GateMutexPri.h>

typedef FIL File;
typedef FRESULT FResult;
typedef SDFatFS_Handle SdHandle;

// Designed to have at most one file open at a given time, in at most one thread
class SdCard {
   public:
    static const byte kFileReadMode = 0x01;
    static const byte kFileOpenExistingMode = 0x00;
    static const byte kFileWriteMode = 0x02;
    static const byte kFileCreateNewMode = 0x04;
    static const byte kFileCreateAlwaysMode = 0x08;
    static const byte kFileOpenAlwaysMode = 0x10;
    static const byte kFileWrittenMode = 0x20;
    static const byte kFileDirtyMode = 0x40;

    static SdCard *GetInstance();
    SdHandle SdOpen();
    void SdClose(SdHandle handle);
    File *FileOpen(const char *path, byte mode);
    void FileClose(File *f);
    uint32_t FileWrite(File *f, const void *write_buffer,
                       uint32_t num_bytes) const;
    uint32_t FileRead(File *f, void *read_buffer, uint32_t num_bytes) const;
    void FileFlush(File *f) const;
    void FileSeek(File *f, uint32_t dest) const;
    void FileDelete(const char *path) const;
    uint32_t FileSize(File *f) const;
    void Format();

   private:
    SdCard();
    static SdCard *instance;
    static const uint8_t kDriveNum = 0;
    SdHandle handle;
    // TODO(dingbenjamin): Timeout the mutex access so a task can't hog the SD
    // card
    GateMutexPri_Params mutex_params;
    GateMutexPri_Handle sd_mutex;
    IArg key;
    bool is_locked;
    File *open_file;
    void Lock();
    void Unlock();
};
#endif  // SRC_DATABASE_SDCARD_H_
