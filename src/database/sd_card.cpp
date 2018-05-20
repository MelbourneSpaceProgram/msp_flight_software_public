#include <external/etl/exception.h>
#include <src/board/board.h>
#include <src/database/sd_card.h>
#include <string.h>
#include <third_party/fatfs/ff.h>
#include <ti/drivers/SDFatFS.h>
#include <time.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

SdHandle SdCard::handle = NULL;

// TODO(dingbenjamin): Parameterize
SdHandle SdCard::SdOpen() {
    handle = SDFatFS_open(0, kDriveNum);
    if (handle == NULL) {
        throw etl::exception("Error starting the SD card.", __FILE__, __LINE__);
    }
    return handle;
}

void SdCard::SdClose(SdHandle handle) { SDFatFS_close(handle); }

File SdCard::FileOpen(const char *path, byte mode) {
    File f;
    FResult result = f_open(&f, path, mode);
    if (result == FR_NO_FILE || result == FR_NO_PATH) {
        Log_error1("Could not find file: %s", (IArg)path);
    } else if (result == FR_EXIST) {
        Log_error1("File already exists: %s", (IArg)path);
    } else if (result != FR_OK) {
        throw etl::exception("Error opening file", __FILE__, __LINE__);
    }
    return f;
}

uint32_t SdCard::FileWrite(File &f, const void *write_buffer,
                           uint32_t num_bytes) {
    uint32_t bytes_written;
    FResult result = f_write(&f, write_buffer, num_bytes, &bytes_written);
    if (result != FR_OK) {
        throw etl::exception("Error writing to file", __FILE__, __LINE__);
    }
    return bytes_written;
}

uint32_t SdCard::FileRead(File &f, void *read_buffer, uint32_t num_bytes) {
    uint32_t bytes_read;
    FResult result = f_read(&f, read_buffer, num_bytes, &bytes_read);
    if (result != FR_OK) {
        throw etl::exception("Error reading from file", __FILE__, __LINE__);
    }
    return bytes_read;
}

void SdCard::FileFlush(File &f) {
    FResult result = f_sync(&f);
    if (result != FR_OK) {
        throw etl::exception("Error flushing to file", __FILE__, __LINE__);
    }
}

void SdCard::FileSeek(File &f, uint32_t dest) {
    FResult result = f_lseek(&f, dest);
    if (result != FR_OK) {
        throw etl::exception("Error seeking in file", __FILE__, __LINE__);
    }
}

void SdCard::FileClose(File &f) {
    FResult result = f_close(&f);
    if (result != FR_OK) {
        throw etl::exception("Error closing file", __FILE__, __LINE__);
    }
}

void SdCard::FileDelete(const char *path) {
    FResult result = f_unlink(path);
    if (result != FR_OK) {
        throw etl::exception("Error deleting file", __FILE__, __LINE__);
    }
}
