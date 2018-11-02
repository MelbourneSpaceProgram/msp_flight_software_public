#include <src/board/MSP432E.h>
#include <src/database/sd_exception.h>
#include <src/database/sd_raid.h>

SdRaid *SdRaid::instance = NULL;

SdRaid *SdRaid::GetInstance() {
    if (instance == NULL) {
        instance = new SdRaid();
    }
    return instance;
}

SdRaid::SdRaid() {
    GateMutexPri_Params_init(&mutex_params);
    raid_mutex = GateMutexPri_create(&mutex_params, NULL);

    // TODO(dingbenjamin): Loop this
    sd_cards[0] = new SdCard(0, SYS_nCS1);
    sd_cards[1] = new SdCard(1, SYS_nCS2);
    sd_cards[2] = new SdCard(2, SYS_nCS3);
    for (uint8_t i = 0; i < kRaidSize; ++i) {
        // TODO(dingbenjamin): Store and load this data in flash memory
        sd_states[i] = kNominal;
    }
}

bool SdRaid::FileCreate(const char *path) {
    IArg key = GateMutexPri_enter(raid_mutex);
    SdHandle handle;
    File *open_file;
    SdCard *active_card;
    for (uint8_t i = 0; i < kRaidSize; ++i) {
        if (sd_states[i] == kNominal) {
            try {
                handle = NULL;
                open_file = NULL;
                active_card = sd_cards[i];
                handle = active_card->SdOpen();
                open_file =
                    active_card->FileOpen(path, SdCard::kFileCreateNewMode);
                active_card->FileClose(open_file);
                active_card->SdClose(handle);
            } catch (SdException &e) {
                // TODO(dingbenjamin): Handle case where returns FR_EXIST on
                // only one
                CloseGracefully(active_card, handle, open_file);
                if (e.f_result == FR_EXIST) {
                    GateMutexPri_leave(raid_mutex, key);
                    return false;
                }

                MspException::LogException(e);
                Log_warning2(
                    "RAID Failure on SD index %d during file creation with "
                    "FatFS "
                    "code %d",
                    i, e.f_result);
                sd_states[i] = kFailed;
            } catch (etl::exception &e) {
                GateMutexPri_leave(raid_mutex, key);
                throw;
            }
        }
    }
    GateMutexPri_leave(raid_mutex, key);
    return IsAlive();
}

bool SdRaid::FileWrite(const char *path, const void *write_buffer,
                       uint32_t location, uint32_t num_bytes) {
    IArg key = GateMutexPri_enter(raid_mutex);
    for (uint8_t i = 0; i < kRaidSize; ++i) {
        if (sd_states[i] == kNominal) {
            SdCard *active_card;
            SdHandle handle;
            File *open_file;
            try {
                handle = NULL;
                open_file = NULL;
                active_card = sd_cards[i];
                handle = active_card->SdOpen();
                File *open_file = active_card->FileOpen(
                    path, SdCard::kFileOpenExistingMode |
                              SdCard::kFileWriteMode | SdCard::kFileReadMode);
                active_card->FileSeek(open_file, location);

                if (active_card->FileWrite(open_file, write_buffer,
                                           num_bytes) != num_bytes) {
                    throw SdException("Wrote incorrect number of bytes", 0,
                                      kFileIncorrectWriteFail, __FILE__,
                                      __LINE__);
                }

                active_card->FileClose(open_file);
                active_card->SdClose(handle);
            } catch (SdException &e) {
                CloseGracefully(active_card, handle, open_file);

                // TODO(dingbenjamin): Handle case where returns FR_EXIST on
                // only one element
                if ((e.f_result == FR_NO_FILE || e.f_result == FR_NO_PATH)) {
                    GateMutexPri_leave(raid_mutex, key);
                    throw;
                }

                MspException::LogException(e);
                Log_warning2(
                    "RAID Failure on SD index %d during file write with FatFS "
                    "code %d",
                    i, e.f_result);
                // If it works for the first one but not subsequent this
                // is an issue
                sd_states[i] = kFailed;
                continue;
            } catch (etl::exception &e) {
                GateMutexPri_leave(raid_mutex, key);
                throw;
            }
        }
    }
    GateMutexPri_leave(raid_mutex, key);
    return IsAlive();
}

bool SdRaid::FileRead(const char *path, void *read_buffer, uint32_t location,
                      uint32_t num_bytes) {
    IArg key = GateMutexPri_enter(raid_mutex);
    // Read from the first one that doesn't fail
    for (uint8_t i = 0; i < kRaidSize; ++i) {
        if (sd_states[i] == kNominal) {
            SdHandle handle;
            File *open_file;
            SdCard *active_card;

            try {
                handle = NULL;
                open_file = NULL;
                active_card = sd_cards[i];
                handle = active_card->SdOpen();
                File *open_file =
                    active_card->FileOpen(path, SdCard::kFileReadMode);
                active_card->FileSeek(open_file, location);

                if (active_card->FileRead(open_file, read_buffer, num_bytes) !=
                    num_bytes) {
                    throw SdException("Read incorrect number of bytes", 0,
                                      kFileIncorrectReadFail, __FILE__,
                                      __LINE__);
                }

                active_card->FileClose(open_file);
                active_card->SdClose(handle);
            } catch (SdException &e) {
                CloseGracefully(active_card, handle, open_file);

                // Not really a RAID error if the file doesn't exist
                // TODO(dingbenjamin): Handle case where returns FR_EXIST on
                // only one element
                if ((e.f_result == FR_NO_FILE || e.f_result == FR_NO_PATH)) {
                    GateMutexPri_leave(raid_mutex, key);
                    throw;
                }

                MspException::LogException(e);
                Log_warning2(
                    "RAID Failure on SD index %d during file read with FatFS "
                    "code %d",
                    i, e.f_result);

                // If it works for the first one but not subsequent this
                // is an issue
                sd_states[i] = kFailed;
                continue;
            } catch (etl::exception &e) {
                GateMutexPri_leave(raid_mutex, key);
                throw;
            }
        }
    }
    GateMutexPri_leave(raid_mutex, key);
    return IsAlive();
}

void SdRaid::FileDelete(const char *path) {}

uint32_t SdRaid::FileSize(File *f) { return 0; }

bool SdRaid::Format(uint8_t index) {
    if (index >= kRaidSize) {
        Log_warning1("SD index %d out of array bounds", index);
        return false;
    }
    IArg key = GateMutexPri_enter(raid_mutex);
    SdCard *active_card = NULL;
    SdHandle handle;
    try {
        handle = NULL;
        active_card = sd_cards[index];
        handle = active_card->SdOpen();
        active_card->Format();
        active_card->SdClose(handle);
    } catch (SdException &e) {
        MspException::LogException(e);
        Log_warning2(
            "RAID Failure on SD index %d during sd format with FatFS "
            "code %d",
            index, e.f_result);

        if (e.f_result == FR_NOT_READY) {
            // Probably a physical error with the card
            // TODO(dingbenjamin): Set this state after multiple attempts
            sd_states[index] = kHardFailed;
        } else {
            MspException::LogException(e);
            sd_states[index] = kFailed;
        }
        return false;
    } catch (etl::exception &e) {
        GateMutexPri_leave(raid_mutex, key);
        throw;
    }

    GateMutexPri_leave(raid_mutex, key);
    return true;
}

bool SdRaid::FormatAll() {
    bool success = true;
    for (uint8_t i = 0; i < kRaidSize; i++) {
        if (!Format(i)) {
            success = false;
            Log_error1("Failed to format at index %d", i);
        }
    }
    return success;
}

bool SdRaid::IsAlive() {
    for (uint8_t i = 0; i < kRaidSize; ++i) {
        if (sd_states[i] == kNominal) {
            return true;
        }
    }
    return false;
}

void SdRaid::CloseGracefully(SdCard *card, SdHandle handle, File *open_file) {
    if (handle != NULL) {
        if (open_file != NULL) {
            try {
                card->FileClose(open_file);
            } catch (SdException &e) {
                MspException::LogException(e);
            }
        }

        try {
            card->SdClose(handle);
        } catch (SdException &e) {
            MspException::LogException(e);
        }
    }
}
