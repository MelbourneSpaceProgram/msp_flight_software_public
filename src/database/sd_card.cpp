#include <assert.h>
#include <external/etl/exception.h>
#include <src/board/board.h>
#include <src/config/unit_tests.h>
#include <src/database/sd_card.h>
#include <string.h>
#include <third_party/fatfs/ff.h>
#include <ti/drivers/SDFatFS.h>
#include <time.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

GateMutexPri_Params SdCard::mutex_params = {NULL};
GateMutexPri_Handle SdCard::sd_mutex = NULL;
bool SdCard::initialised = false;
SdHandle SdCard::handle = NULL;
File *SdCard::open_file = NULL;
IArg SdCard::key = NULL;
bool SdCard::is_locked = false;

void SdCard::Init() {
    if (!initialised) {
        GateMutexPri_Params_init(&mutex_params);
        sd_mutex = GateMutexPri_create(&mutex_params, NULL);
        if (sd_mutex == NULL) {
            throw etl::exception("Failed to create SD card mutex", __FILE__,
                                 __LINE__);
        }
        initialised = true;
    }
}

// TODO(dingbenjamin): Parameterize
SdHandle SdCard::SdOpen() {
    assert(initialised);
    if (!kSdCardAvailable) {
        Log_info0("SdCard not available");
    }

    handle = SDFatFS_open(0, kDriveNum);
    if (handle == NULL) {
        throw etl::exception("Error starting the SD card.", __FILE__, __LINE__);
    }
    return handle;
}

void SdCard::SdClose(SdHandle handle) {
    assert(initialised);
    if (is_locked) {
        throw etl::exception("Cannot close SD card, there is a file open",
                             __FILE__, __LINE__);
    }
    SDFatFS_close(handle);
    GateMutexPri_leave(sd_mutex, key);
}

// TODO(dingbenjamin): Print file name in exceptions
File *SdCard::FileOpen(const char *path, byte mode) {
    assert(initialised);
    Lock();
    open_file = new File;
    FResult result = f_open(open_file, path, mode);
    if (result == FR_NO_FILE || result == FR_NO_PATH) {
        Unlock();
        throw etl::exception("Could not find file", __FILE__, __LINE__);
    } else if (result == FR_EXIST) {
        Log_error1("File already exists: %s", (IArg)path);
    } else if (result != FR_OK) {
        Unlock();
        throw etl::exception("Error opening file", __FILE__, __LINE__);
    }
    return open_file;
}

uint32_t SdCard::FileWrite(File *f, const void *write_buffer,
                           uint32_t num_bytes) {
    // Don't need a lock check as the file handle itself is the key
    assert(initialised);
    uint32_t bytes_written;
    FResult result = f_write(f, write_buffer, num_bytes, &bytes_written);
    if (result != FR_OK) {
        throw etl::exception("Error writing to file", __FILE__, __LINE__);
    }
    return bytes_written;
}

uint32_t SdCard::FileRead(File *f, void *read_buffer, uint32_t num_bytes) {
    // Don't need a lock check as the file handle itself is the key
    assert(initialised);
    uint32_t bytes_read;
    FResult result = f_read(f, read_buffer, num_bytes, &bytes_read);
    if (result != FR_OK) {
        throw etl::exception("Error reading from file", __FILE__, __LINE__);
    }
    return bytes_read;
}

void SdCard::FileFlush(File *f) {
    // Don't need a lock check as the file handle itself is the key
    assert(initialised);
    FResult result = f_sync(f);
    if (result != FR_OK) {
        throw etl::exception("Error flushing to file", __FILE__, __LINE__);
    }
}

void SdCard::FileSeek(File *f, uint32_t dest) {
    // Don't need a lock check as the file handle itself is the key
    assert(initialised);
    FResult result = f_lseek(f, dest);
    if (result != FR_OK) {
        throw etl::exception("Error seeking in file", __FILE__, __LINE__);
    }
}

void SdCard::FileClose(File *f) {
    assert(initialised);
    if (open_file == NULL) {
        throw etl::exception("No file currently open", __FILE__, __LINE__);
    } else if (f != open_file) {
        throw etl::exception("Must close the currently open file", __FILE__,
                             __LINE__);
    }
    FResult result = f_close(f);
    if (result != FR_OK) {
        throw etl::exception("Error closing file", __FILE__, __LINE__);
    }
    delete open_file;
    open_file = NULL;
    Unlock();
}

void SdCard::FileDelete(const char *path) {
    assert(initialised);
    if (is_locked)
        throw etl::exception("Cannot delete files while a file is open",
                             __FILE__, __LINE__);
    FResult result = f_unlink(path);
    if (result != FR_OK) {
        throw etl::exception("Error deleting file", __FILE__, __LINE__);
    }
}

uint32_t SdCard::FileSize(File *f) {
    assert(initialised);
    FSIZE_t size = f_size(f);
    return static_cast<uint32_t>(size);
}

void SdCard::Format() {
    assert(initialised);
    Lock();
    byte work[FF_MAX_SS];
    Log_info0("Formatting SD Card");
    FResult result = f_mkfs("0", FM_ANY, 0, work, sizeof(work));
    Unlock();
    if (result != FR_OK) {
        throw etl::exception("Could not format SdCard", __FILE__, __LINE__);
    }
    Log_info0("SD Card Formatted");
}

void SdCard::Lock() {
    key = GateMutexPri_enter(sd_mutex);
    assert(!is_locked);
    if (is_locked) {
        throw etl::exception(
            "Something else is using the sd card in the same thread", __FILE__,
            __LINE__);
    }
    is_locked = true;
}

void SdCard::Unlock() {
    assert(is_locked);
    if (!is_locked) {
        Log_error0("Tried to unlock the SD card when it was already unlocked");
        return;
    }
    GateMutexPri_leave(sd_mutex, key);
    is_locked = false;
    key = NULL;
}
