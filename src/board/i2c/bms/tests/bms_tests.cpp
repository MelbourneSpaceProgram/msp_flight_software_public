#include <CppUTest/TestHarness.h>
#include <external/etl/array.h>
#include <src/board/board.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>

// Address value must be changed for each BMS.
static const byte test_bms_address = 0x68;
static const uint8_t kReadRegisterCount = 2;

// TODO(hugorilla): Complete further testing in coordination with electronics team

TEST_GROUP(Bms) {
    void setup() {
        if (!kBmsAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(Bms, TestBms) {
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);
    etl::array<byte, kReadRegisterCount> read_buffer;

    CHECK_EQUAL(Bms::kVinUvclSettingConfigurationValue,
                bms.GetVinUvclSetting());
    CHECK_EQUAL(Bms::kVchargeSettingConfigurationValue,
                bms.GetVchargeSetting());
    CHECK_EQUAL(Bms::kIChargeTargetConfigurationValue, bms.GetIchargeTarget());
    CHECK_EQUAL((Bms::kRechargeThresholdConfigurationUBValue << 8 |
                 Bms::kRechargeThresholdConfigurationLBValue),
                bms.GetRechargeThreshold());

    CHECK(Bms::kError != bms.GetChargeStatus());
}

TEST(Bms, TestBmsDieTemperatureRead) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kEpsBmsDieT1, 1);
    DOUBLES_EQUAL(25, temp, 50);
}

TEST(Bms, TestBmsBatteryTemperatureRead) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kEpsBmsBatT1, 1);
    DOUBLES_EQUAL(25, temp, 50);
}

TEST(Bms, TestJeitaRegion) {
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);

    uint16_t jeita_region = 3;
    double v_charge = 3.6;
    double i_charge = 1.0;

    CHECK_EQUAL(jeita_region, bms.GetJeitaRegion());
    DOUBLES_EQUAL(v_charge, bms.GetVchargeDac(), 0.5);
    DOUBLES_EQUAL(i_charge, bms.GetIchargeDac(), 0.5);
}
