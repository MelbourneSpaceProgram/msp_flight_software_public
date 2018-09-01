#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>

TEST_GROUP(CurrentSensor) {
    void setup() {
        if (!kCurrentSensorTestEnabled) {
            TEST_EXIT;
        }
    };
};

// TODO(dingbenjamin): Write the current sensor test
