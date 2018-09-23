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

    static void Init();
    static SdHandle SdOpen();
    static void SdClose(SdHandle handle);
    static File *FileOpen(const char *path, byte mode);
    static void FileClose(File *f);
    static uint32_t FileWrite(File *f, const void *write_buffer,
                              uint32_t num_bytes);
    static uint32_t FileRead(File *f, void *read_buffer, uint32_t num_bytes);
    static void FileFlush(File *f);
    static void FileSeek(File *f, uint32_t dest);
    static void FileDelete(const char *path);
    static uint32_t FileSize(File *f);
    static void Format();

   private:
    static const uint8_t kDriveNum = 0;
    static SdHandle handle;
    // TODO(dingbenjamin): Timeout the mutex access so a task can't hog the SD
    // card
    static GateMutexPri_Params mutex_params;
    static GateMutexPri_Handle sd_mutex;
    static IArg key;
    static bool initialised;
    static bool is_locked;
    static File *open_file;
    static void Lock();
    static void Unlock();
};
#endif  // SRC_DATABASE_SDCARD_H_
