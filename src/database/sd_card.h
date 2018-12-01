#ifndef SRC_DATABASE_SDCARD_H_
#define SRC_DATABASE_SDCARD_H_

#include <src/board/uart/uart.h>
#include <src/util/data_types.h>
#include <ti/drivers/SD.h>
#include <ti/drivers/SDFatFS.h>
#include <ti/sysbios/gates/GateMutexPri.h>

typedef FIL File;
typedef FRESULT FResult;
typedef SDFatFS_Handle SdHandle;

// Designed to have at most one file open at a given time, in at most one thread
class SdCard {
    friend class FileLocker;

    class FileLocker() {
       public:
        explicit FileLocker(SdCard & sd, File * file);
        ~FileLocker();

        File *GetFile() const;

        FileLocker(const FileLocker &) = delete;
        FileLocker &operator=(const FileLocker &) = delete;
        FileLocker(FileLocker &&) = delete;
        FileLocker &operator=(FileLocker &&) = delete;

       private:
        SdCard &sd;
        GateMutexPri_Handle &handle;
        IArg key;
        File *file;
    }

   public:
    static constexpr byte kFileReadMode = 0x01;
    static constexpr byte kFileOpenExistingMode = 0x00;
    static constexpr byte kFileWriteMode = 0x02;
    static constexpr byte kFileCreateNewMode = 0x04;
    static constexpr byte kFileCreateAlwaysMode = 0x08;
    static constexpr byte kFileOpenAlwaysMode = 0x10;
    static constexpr byte kFileWrittenMode = 0x20;
    static constexpr byte kFileDirtyMode = 0x40;
    static constexpr uint8_t kFileNameLength = 3;

    static SdCard *GetInstance();
    SdHandle SdOpen();
    void SdClose(SdHandle handle);
    FileLocker *FileOpen(const char *path, byte mode);
    uint32_t FileWrite(File *f, const void *write_buffer,
                       uint32_t num_bytes) const;
    uint32_t FileRead(File *f, void *read_buffer, uint32_t num_bytes) const;
    void FileFlush(File *f) const;
    void FileSeek(File *f, uint32_t dest) const;
    void FileDelete(const char *path) const;
    uint32_t FileSize(File *f) const;
    void Format();
    void Dump();

   private:
    SdCard();
    void FileClose(File *f);
    static SdCard *instance;
    static constexpr uint8_t kDriveNum = 0;
    static constexpr uint16_t kMaxNumFiles = 1000;
    static constexpr uint8_t kMessageSize = 255;
    SdHandle handle;
    // TODO(dingbenjamin): Timeout the mutex access so a task can't hog the SD
    // card
    GateMutexPri_Params mutex_params;
    GateMutexPri_Handle sd_mutex;
    bool is_locked;
    File *open_file;
    void Lock();
    void Unlock();
    void FileDump(File *src, char *fpath);
};
#endif  // SRC_DATABASE_SDCARD_H_
