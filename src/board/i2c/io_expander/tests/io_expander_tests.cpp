#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/io_expander/io_expander.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/sensors/measurable_manager.h>
#include <src/util/data_types.h>
#include <src/util/satellite_power.h>
#include <src/util/tirtos_utils.h>

TEST_GROUP(IoExpander) {
    void setup() {
        if (!SystemConfiguration::GetInstance()->IsI2cAvailable()) {
            TEST_EXIT;
        }
    };
};

TEST(IoExpander, TestIoExpander) {
    const IoExpander *bms_io_expander =
        IoExpander::GetIoExpander(IoExpander::kEpsIoExpander);
    MeasurableManager *measurable_manager = MeasurableManager::GetInstance();
    MagnetometerReading magnetometer_reading;

    bms_io_expander->SetPin(SatellitePower::kIoExpanderPinFSEn, false);
    TirtosUtils::SleepMilli(2000);

    magnetometer_reading =
        measurable_manager->ReadNanopbMeasurable<MagnetometerReading>(
            kFsImuMagno2, 0);
    CHECK_EQUAL(magnetometer_reading.x, kInvalidDouble);
    CHECK_EQUAL(magnetometer_reading.y, kInvalidDouble);
    CHECK_EQUAL(magnetometer_reading.z, kInvalidDouble);

    bms_io_expander->SetPin(SatellitePower::kIoExpanderPinFSEn, true);
    TirtosUtils::SleepMilli(2000);

    magnetometer_reading =
        measurable_manager->ReadNanopbMeasurable<MagnetometerReading>(
            kFsImuMagno2, 0);

    CHECK(magnetometer_reading.x != kInvalidDouble);
    CHECK(magnetometer_reading.y != kInvalidDouble);
    CHECK(magnetometer_reading.z != kInvalidDouble);
}
