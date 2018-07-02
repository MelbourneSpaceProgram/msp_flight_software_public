#include <CppUTest/TestHarness.h>
#include <external/etl/exception.h>
#include <src/config/unit_tests.h>
#include <src/database/sd_card.h>
#include <string.h>
#include <xdc/runtime/Log.h>

const char input_file[] = "0:test_in.txt";
const char output_file[] = "0:test_out.txt";

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

TEST_GROUP(SdCard){void setup(){if (!sd_card_available){TEST_EXIT}}};
TEST(SdCard, FatFsReadWrite) {
    File src = SdCard::FileOpen(input_file, SdCard::kFileWriteMode |
                                                SdCard::kFileReadMode |
                                                SdCard::kFileCreateAlwaysMode);
    CHECK_EQUAL(strlen(text_array),
                SdCard::FileWrite(src, text_array, strlen(text_array)));
    SdCard::FileFlush(src);
    SdCard::FileSeek(src, 0);
    SdCard::FileClose(src);

    src = SdCard::FileOpen(input_file, SdCard::kFileReadMode);

    File dst = SdCard::FileOpen(
        output_file, SdCard::kFileCreateAlwaysMode | SdCard::kFileWriteMode);
    char copy_buffer[copy_buffer_size + 1];

    // Copy the contents from the src to the dst
    uint32_t bytes_read, bytes_written;
    uint64_t total_bytes_copied = 0;
    while (true) {
        bytes_read = SdCard::FileRead(src, copy_buffer, copy_buffer_size);
        if (bytes_read == 0) {
            break;
        }

        bytes_written = SdCard::FileWrite(dst, copy_buffer, bytes_read);
        if (bytes_written < bytes_read) {
            Log_error0("SD Card full");
            break;
        }
        total_bytes_copied += bytes_written;
    }

    SdCard::FileFlush(dst);

    // TODO(akremor): Need to develop a fix for the below
    // It is API version dependent which means it breaks some builds
    // uint32_t filesize = src.fsize;
    uint32_t filesize = 0;

    SdCard::FileClose(src);
    SdCard::FileClose(dst);

    dst = SdCard::FileOpen(output_file, SdCard::kFileReadMode);

    uint16_t index = 0;
    while (true) {
        uint32_t bytes_read =
            SdCard::FileRead(dst, copy_buffer, copy_buffer_size);
        if (bytes_read == 0) {
            break;  // EOF
        }
        copy_buffer[bytes_read] = '\0';
        STRNCMP_EQUAL(text_array + index, copy_buffer, bytes_read);
        index += copy_buffer_size;
    }

    SdCard::FileClose(dst);
    SdCard::FileDelete(input_file);
    SdCard::FileDelete(output_file);
}
