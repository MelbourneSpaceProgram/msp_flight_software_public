#include <CppUTest/TestHarness.h>
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

TEST_GROUP(CacheWriteBack) {
    void setup() {
        if (!sd_card_available || !i2c_available) {
            TEST_EXIT;
        }
    };
};

// WARNING: Test deletes SD file for CdhTemp1
TEST(CacheWriteBack, TestCacheWriteBack) {
    char filename[3];
    snprintf(filename, sizeof(filename), "%03d", kCdhT);

    I2cMeasurableManager *manager = I2cMeasurableManager::GetInstance();

    try {
        SdCard::FileDelete(filename);
    } catch (etl::exception e) {
        // Probably just means the file doesn't exist, this is ok
    }

    // Populate the cache with a new reading
    float temp = manager->ReadI2cMeasurable<double>(kCdhT, 0);

    // The same value in `temp` should now be in the cache, so write the cache
    // to SD card
    try {
        RunnableCacheWriteBack::WriteBackTemp(kCdhT);
    } catch (etl::exception e) {
        // Likely SD card not present
        FAIL("Uncaught exception in test");
    }

    // Check the SD card for the same value as `temp`
    TemperatureReading temp_message =
        CircularBufferNanopb(TemperatureReading)::ReadMessage(filename);
    DOUBLES_EQUAL(temp_message.temp, temp, 0.001);

    // Delete the file now that test is complete
    try {
        SdCard::FileDelete(filename);
    } catch (etl::exception e) {
        FAIL("Failed to delete file after write back test");
    }
}
