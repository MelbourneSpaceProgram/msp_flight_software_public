#include <external/etl/exception.h>
#include <external/nanopb/pb_decode.h>
#include <external/nanopb/pb_encode.h>
#include <src/config/unit_tests.h>
#include <src/database/circular_buffer_nanopb.h>
#include <src/database/sd_card.h>
#include <src/messages/TemperatureReading.pb.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <src/sensors/measurable_id.h>
#include <src/sensors/runnable_cache_write_back.h>
#include <stdio.h>
#include <test_runners/unity.h>

// WARNING: Test deletes SD file for CdhTemp1
void TestCacheWriteBack() {
    if (!write_back_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }

    char filename[3];
    snprintf(filename, sizeof(filename), "%03d", kCdhTemp1);

    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();

    try {
        SdCard::FileDelete(filename);
    } catch (etl::exception e) {
        // Probably just means the file doesn't exist, this is ok
    }

    // Populate the cache with a new reading
    float temp = manager->ReadI2cMeasurable<double>(kCdhTemp1, 0);

    // The same value in `temp` should now be in the cache, so write the cache
    // to SD card
    RunnableCacheWriteBack::WriteBackTemp(kCdhTemp1);

    // Check the SD card for the same value as `temp`
    TemperatureReading temp_message =
        CircularBufferNanopb(TemperatureReading)::ReadMessage(filename);
    TEST_ASSERT_EQUAL_FLOAT(temp_message.temp, temp);

    // Delete the file now that test is complete
    try {
        SdCard::FileDelete(filename);
    } catch (etl::exception e) {
        TEST_ASSERT_MESSAGE(false,
                            "Failed to delete file after write back test");
    }
}
