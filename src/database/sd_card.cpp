#include <assert.h>
#include <external/etl/exception.h>
#include <src/board/MSP432E.h>
#include <src/board/board.h>
#include <src/config/satellite.h>
#include <src/config/unit_tests.h>
#include <src/database/sd_card.h>
#include <src/database/sd_exception.h>
#include <src/sensors/measurable_id.h>
#include <src/util/message_codes.h>
#include <src/util/msp_exception.h>
#include <src/util/runnable_console_logger.h>
#include <src/util/task_utils.h>
#include <stdio.h>
#include <string.h>
#include <third_party/fatfs/ff.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SDFatFS.h>
#include <time.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

SdCard::SdCard(uint8_t sd_index, uint8_t gpio_index)
    : handle(NULL),
      key(NULL),
      is_locked(NULL),
      open_file(NULL),
      sd_index(sd_index),
      gpio_index(gpio_index) {}

SdHandle SdCard::SdOpen() {
    if (!kSdCardAvailable) {
        Log_info0("SdCard not available");
    }

    assert(sd_index < MSP_EXP432P401R_SDCOUNT);
    GPIO_write(gpio_index, 0);
    TaskUtils::SleepMilli(5);
    handle = SDFatFS_open(sd_index, kDriveNum);
    if (handle == NULL) {
        throw SdException("Error starting the SD card.", 0, kSdOpenFail,
                          __FILE__, __LINE__);
    }
    return handle;
}

void SdCard::SdClose(SdHandle handle) {
    if (is_locked) {
        throw SdException("Cannot close SD card, there is a file open", 0,
                          kFileCloseLockFail, __FILE__, __LINE__);
    }
    SDFatFS_close(handle);
    TaskUtils::SleepMilli(5);
    GPIO_write(gpio_index, 1);
}

// TODO(dingbenjamin): Print file name in exceptions
File *SdCard::FileOpen(const char *path, byte mode) {
    Lock();
    open_file = new File;
    FResult result = f_open(open_file, path, mode);
    if (result == FR_NO_FILE || result == FR_NO_PATH) {
        delete open_file;
        open_file = NULL;
        Unlock();
        throw SdException("Could not find file", 0, kFileOpenNotFoundFail,
                          __FILE__, __LINE__, result);
    } else if (result == FR_EXIST) {
        // TODO(dingbenjamin): Add filename
        delete open_file;
        open_file = NULL;
        Unlock();
        throw SdException("File already exists", 0, kFileOpenExistsFail,
                          __FILE__, __LINE__, result);
    } else if (result != FR_OK) {
        delete open_file;
        open_file = NULL;
        Unlock();
        throw SdException("Error opening file", 0, kFileOpenFail, __FILE__,
                          __LINE__, result);
    }
    return open_file;
}

uint32_t SdCard::FileWrite(File *f, const void *write_buffer,
                           uint32_t num_bytes) const {
    // Don't need a lock check as the file handle itself is the key
    uint32_t bytes_written;
    FResult result = f_write(f, write_buffer, num_bytes, &bytes_written);
    if (result != FR_OK) {
        throw SdException("Error writing to file", 0, kFileWriteFail, __FILE__,
                          __LINE__, result);
    }
    return bytes_written;
}

uint32_t SdCard::FileRead(File *f, void *read_buffer,
                          uint32_t num_bytes) const {
    // Don't need a lock check as the file handle itself is the key
    uint32_t bytes_read;
    FResult result = f_read(f, read_buffer, num_bytes, &bytes_read);
    if (result != FR_OK) {
        throw SdException("Error reading from file", 0, kFileReadFail, __FILE__,
                          __LINE__, result);
    }
    return bytes_read;
}

void SdCard::FileFlush(File *f) const {
    // Don't need a lock check as the file handle itself is the key
    FResult result = f_sync(f);
    if (result != FR_OK) {
        throw SdException("Error flushing to file", 0, kFileFlushFail, __FILE__,
                          __LINE__, result);
    }
}

void SdCard::FileSeek(File *f, uint32_t dest) const {
    // Don't need a lock check as the file handle itself is the key
    FResult result = f_lseek(f, dest);
    if (result != FR_OK) {
        throw SdException("Error seeking in file", 0, kFileSeekFail, __FILE__,
                          __LINE__, result);
    }
}

void SdCard::FileClose(File *f) {
    if (open_file == NULL) {
        throw SdException("No file currently open", 0, kFileCloseNullFail,
                          __FILE__, __LINE__);
    } else if (f != open_file) {
        throw SdException("Must close the currently open file", 0,
                          kFileCloseWrongFail, __FILE__, __LINE__);
    }
    FResult result = f_close(f);
    if (result != FR_OK) {
        throw SdException("Error closing file", 0, kFileCloseFail, __FILE__,
                          __LINE__);
    }
    delete open_file;
    open_file = NULL;
    Unlock();
}

void SdCard::FileDelete(const char *path) const {
    if (is_locked)
        throw SdException("Cannot delete files while a file is open", 0,
                          kFileDeleteOpenFail, __FILE__, __LINE__);
    FResult result = f_unlink(path);
    if (result != FR_OK) {
        throw SdException("Error deleting file", 0, kFileDeleteFail, __FILE__,
                          __LINE__);
    }
}

uint32_t SdCard::FileSize(File *f) const {
    FSIZE_t size = f_size(f);
    return static_cast<uint32_t>(size);
}

void SdCard::Format() {
    Lock();
    byte work[FF_MAX_SS];
    Log_info0("Formatting SD Card");

    // Extra character for the null terminator
    char fvol[2 * sizeof(char)];
    sprintf(fvol, "%1d", sd_index);

    FResult result = f_mkfs(fvol, FM_ANY, 0, work, sizeof(work));
    Unlock();
    if (result != FR_OK) {
        throw SdException("Could not format SdCard", 0, kFileFormatFail,
                          __FILE__, __LINE__, result);
    }
    Log_info0("SD Card Formatted");
}

void SdCard::Lock() {
    assert(!is_locked);
    if (is_locked) {
        throw SdException(
            "Something else is using the sd card in the same thread", 0,
            kFileThreadFail, __FILE__, __LINE__);
    }
    is_locked = true;
}

void SdCard::Unlock() {
    assert(is_locked);
    if (!is_locked) {
        Log_error0("Tried to unlock the SD card when it was already unlocked");
        return;
    }
    is_locked = false;
    key = NULL;
}

void SdCard::Dump() {
    for (uint16_t i = 0; i < kMaxNumFiles; i++) {
        // TOOD(dingbenjamin): const fpath after changing WriteToDataLogger
        char fpath[kFileNameLength + 1];
        sprintf(fpath, "%03hu", i);
        const char *file_path = fpath;

        File *src;
        if (kVerboseUnitTests)
            Log_info1("Dump: Opening file %s", (IArg)file_path);
        try {
            src = FileOpen(file_path, SdCard::kFileReadMode);
        } catch (SdException &e) {
            if (e.f_result == FR_NO_FILE) {
                // File not found
                continue;
            }
            throw;
        }

        if (src != NULL) {
            try {
                FileDump(src, fpath);
                FileClose(src);
            } catch (SdException &e) {
                MspException::LogException(e);
                return;
            }
        }
    }
}

void SdCard::FileDump(File *src, char *fpath) {
    uint32_t file_size = FileSize(src);
    uint32_t index = 0;
    byte copy_buffer[kMessageSize];

    RunnableConsoleLogger::WriteToDataLogger(MeasurableId::kSdCardDumpMessage,
                                             reinterpret_cast<byte *>(fpath),
                                             kFileNameLength);
    while (index < file_size) {
        uint32_t bytes_read = FileRead(src, copy_buffer, kMessageSize);

        index += bytes_read;
        if (bytes_read > 0) {
            RunnableConsoleLogger::WriteToDataLogger(
                MeasurableId::kSdCardDumpMessage, copy_buffer, bytes_read);
        } else {
            Log_warning3("File %s: %u bytes read out of %u", (IArg)fpath, index,
                         file_size);
            break;
        }
    }
}
