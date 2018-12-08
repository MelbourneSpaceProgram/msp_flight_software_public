#include <CppUTest/TestHarness.h>
#include <ctype.h>
#include <src/config/unit_tests.h>
#include <src/telecomms/fec/rs8.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

void DoTest(int32_t error_chance);
void PrintArray(uint8_t *array, uint32_t length);
int32_t CompareArrays(uint8_t *array1, uint8_t *array2, int32_t length);
void CorruptArray(uint8_t *array, int32_t length, int32_t chance);

constexpr uint32_t kFecIterations = 10;
constexpr uint32_t kRandomSeed = 1234;

TEST_GROUP(Fec){};

TEST(Fec, ReedSolomon) {
    // Intializes random number generator
    srand(kRandomSeed);

    int32_t i;

    for (i = 0; i <= 100; i++) {
        if (SystemConfiguration::GetInstance()->IsVerboseLogging())
            Log_info2(
                "Testing packet with %d%% chance of corruption %d times ... ",
                i, kFecIterations);

        for (uint32_t j = 0; j < kFecIterations; j++) {
            DoTest(i);
        }
    }
}

void DoTest(int32_t error_chance) {
    // Test payload - 255 bytes long
    // First 223 bytes: Data
    // Last 32 bytes: Parity

    uint8_t test_payload[Rs8::kBlockLength];
    uint8_t corrupt_payload[Rs8::kBlockLength];

    memset(test_payload, 0, Rs8::kBlockLength);
    memset(corrupt_payload, 0, Rs8::kBlockLength);

    // Populate the payload with random data
    CorruptArray(test_payload, Rs8::kDataLength, 100);

    if (SystemConfiguration::GetInstance()->IsVerboseLogging()) {
        PrintArray(test_payload, Rs8::kBlockLength);
        Log_info0("Calculating parity ...\n");
    }

    // Populate the polarity region with RS parity
    Rs8::Encode(test_payload, test_payload + Rs8::kDataLength);

    if (SystemConfiguration::GetInstance()->IsVerboseLogging()) {
        PrintArray(test_payload, Rs8::kBlockLength);
        Log_info0("Copy array ...\n");
    }

    memcpy(corrupt_payload, test_payload, Rs8::kBlockLength * sizeof(uint8_t));

    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
        Log_info0("Corrupting copy ...");

    // Create some random errors in the copy
    CorruptArray(corrupt_payload, Rs8::kBlockLength, error_chance);

    // Print the corrupted array
    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
        PrintArray(corrupt_payload, Rs8::kBlockLength);

    // Count the number of errors that were created
    int32_t corrupt_count =
        CompareArrays(test_payload, corrupt_payload, Rs8::kBlockLength);

    // Print the corrupted copy
    if (SystemConfiguration::GetInstance()->IsVerboseLogging()) {
        Log_info1("Bytes corrupted: %d", corrupt_count);
        Log_info0("Decoding corrupted payload ...");
    }

    // Decode the payload
    int32_t corrected_count = Rs8::Decode(corrupt_payload, NULL, 0);

    // Print the fixed array
    if (SystemConfiguration::GetInstance()->IsVerboseLogging())
        PrintArray(corrupt_payload, Rs8::kBlockLength);

    // Compare the fixed array and compare it to the original array
    int32_t corrupt_count_after_decode =
        CompareArrays(test_payload, corrupt_payload, Rs8::kBlockLength);

    if (SystemConfiguration::GetInstance()->IsVerboseLogging()) {
        Log_info1("Diff from original after decode: %d",
                  corrupt_count_after_decode);
        Log_info1("Errors corrected: %d", corrected_count);
    }

    // Make sure the error count matches the number of errors we introduced
    if (corrected_count == corrupt_count) {
        CHECK_EQUAL_TEXT(0, corrupt_count_after_decode,
                         "Corrupt count not 0 after decode");
    } else if (corrected_count < 0) {
        CHECK_TEXT(
            static_cast<uint8_t>(corrupt_count) > Rs8::kParityLength / 2u,
            "Decoder failed, but number of errors was not above threshold")
    } else {
        char fail_string[200];
        snprintf(fail_string, sizeof(fail_string),
                 "Corrupt count (%d) and corrected count (%d) are not equal. "
                 "Errors remaining in packet: %d",
                 static_cast<int16_t>(corrupt_count),
                 static_cast<int16_t>(corrected_count),
                 static_cast<int16_t>(corrupt_count_after_decode));
        FAIL(fail_string);
    }
}

void PrintArray(uint8_t *array, uint32_t length) {
    uint32_t i;
    for (i = 0; i < length; i++) {
        if ((i % 16) == 0) {
            System_printf("\n");
        }
        System_printf("0x%02x,", array[i]);
    }
    System_printf("\n");
}

int32_t CompareArrays(uint8_t *array1, uint8_t *array2, int32_t length) {
    int32_t diff_count = 0;
    int32_t i;
    for (i = 0; i < length; i++) {
        if (array1[i] != array2[i]) {
            diff_count++;
        }
    }
    return diff_count;
}

void CorruptArray(uint8_t *array, int32_t length, int32_t chance) {
    int32_t i;
    for (i = 0; i < length; i++) {
        if (chance > 0 && (chance >= 100 || (rand() % 99) < chance)) {
            array[i] ^= (rand() % 254) + 1;
        }
    }
}
