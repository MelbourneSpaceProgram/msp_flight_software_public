#include <CppUTest/TestHarness.h>
#include <external/etl/exception.h>
#include <src/config/unit_tests.h>
#include <src/database/sd_card.h>
#include <src/util/msp_exception.h>
#include <string.h>
#include <xdc/runtime/Log.h>

const char input_file[] = "0:test_in.txt";
const char output_file[] = "0:test_out.txt";
const char dump_file_1[] = "999";
const char dump_file_2[] = "998";

const char text_array[] =
    "**********************************************************************"
    "*\n"
    "0         1         2         3         4         5         6         "
    "7\n"
    "0123456789012345678901234567890123456789012345678901234567890123456789"
    "0\n"
    "This is some text to be inserted into the inputfile if there isn't\n"
    "already an existing file located on the media.\n"
    "If an inputfile already exists, or if the file was already once\n"
    "generated, then the inputfile will NOT be modified.\n"
    "**********************************************************************"
    "*\n";

const uint16_t copy_buffer_size = 128;

TEST_GROUP(SdCard) {
    void setup() {
        if (!kSdCardAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(SdCard, FatFsReadWrite) {
    try {
        File *src, *dst;
        SdCard *sd = SdCard::GetInstance();
        src = sd->FileOpen(input_file, SdCard::kFileWriteMode |
                                           SdCard::kFileReadMode |
                                           SdCard::kFileCreateAlwaysMode);

        CHECK_EQUAL(strlen(text_array),
                    sd->FileWrite(src, text_array, strlen(text_array)));
        sd->FileFlush(src);
        sd->FileSeek(src, 0);
        sd->FileClose(src);

        char copy_buffer[copy_buffer_size + 1];

        // Copy the contents from the src to the dst
        uint64_t total_bytes_copied = 0;
        while (true) {
            src = sd->FileOpen(input_file, SdCard::kFileReadMode);
            sd->FileSeek(src, total_bytes_copied);
            uint32_t bytes_read =
                sd->FileRead(src, copy_buffer, copy_buffer_size);
            sd->FileClose(src);

            if (bytes_read == 0) {
                break;
            }

            dst = sd->FileOpen(output_file, SdCard::kFileCreateAlwaysMode |
                                                SdCard::kFileWriteMode);
            sd->FileSeek(dst, total_bytes_copied);
            uint32_t bytes_written =
                sd->FileWrite(dst, copy_buffer, bytes_read);
            sd->FileFlush(dst);
            sd->FileClose(dst);

            if (bytes_written < bytes_read) {
                Log_error0("SD Card full");
                break;
            }
            total_bytes_copied += bytes_written;
        }

        // -1 required to account for the NULL terminator that is in the char
        // array but not written to file
        src = sd->FileOpen(input_file, SdCard::kFileReadMode);
        CHECK_EQUAL(sizeof(text_array) / sizeof(char) - 1, sd->FileSize(src));
        sd->FileClose(src);

        dst = sd->FileOpen(output_file, SdCard::kFileReadMode);

        uint16_t index = 0;
        while (true) {
            uint32_t bytes_read =
                sd->FileRead(dst, copy_buffer, copy_buffer_size);
            if (bytes_read == 0) {
                break;  // EOF
            }
            copy_buffer[bytes_read] = '\0';
            STRNCMP_EQUAL(text_array + index, copy_buffer, bytes_read);
            index += copy_buffer_size;
        }

        sd->FileClose(dst);
        sd->FileDelete(input_file);
        sd->FileDelete(output_file);
    } catch (etl::exception &e) {
        MspException::LogException(e);
        FAIL("Uncaught exception in test");
    }
}

TEST(SdCard, SdDump) {
    SdCard *sd_card = SdCard::GetInstance();
    File *test_1;
    File *test_2;

    try {
        test_1 =
            sd_card->FileOpen(dump_file_1, SdCard::kFileWriteMode |
                                               SdCard::kFileCreateAlwaysMode);
        sd_card->FileWrite(test_1, text_array, strlen(text_array));
        sd_card->FileFlush(test_1);
        sd_card->FileClose(test_1);

        test_2 =
            sd_card->FileOpen(dump_file_2, SdCard::kFileWriteMode |
                                               SdCard::kFileCreateAlwaysMode);
        sd_card->FileWrite(test_2, text_array, strlen(text_array));
        sd_card->FileFlush(test_2);
        sd_card->FileClose(test_2);

        sd_card->Dump();
    } catch (etl::exception &e) {
        // Likely SD card missing
        std::string error(e.what());
        FAIL(("Exception in SdDump " + error).c_str());
    }
}
