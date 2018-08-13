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

TEST_GROUP(Bms) {
    void setup() {
        if (!bms_available) {
            TEST_EXIT;
        }
    };
};

TEST(Bms, TestBms) {
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);
    etl::array<byte, kReadRegisterCount> read_buffer;

    CHECK_EQUAL(Bms::kUVCLRegisterValue,
                bms.GetConfiguration(0x16, read_buffer));
    CHECK_EQUAL(Bms::kVChargeSettingRegisterValue,
                bms.GetConfiguration(0x1B, read_buffer));
    CHECK_EQUAL(Bms::kIChargeRegisterValue,
                bms.GetConfiguration(0x1A, read_buffer));
    CHECK_EQUAL((Bms::kReChargeThresholdURegisterValue << 8 |
                 Bms::kReChargeThresholdLRegisterValue),
                bms.GetConfiguration(0x2E, read_buffer));

    CHECK(Bms::kError != bms.GetChargeStatus(read_buffer));
    CHECK(Bms::kOther != bms.GetSystemStatus(read_buffer));
}

TEST(Bms, TestBmsDieTemperatureRead) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerBmsDieTemp1, 1);
    DOUBLES_EQUAL(25, temp, 50);
}

TEST(Bms, TestBmsBatteryTemperatureRead) {
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerBmsBatteryTemp1, 1);
    DOUBLES_EQUAL(25, temp, 50);
}

TEST(Bms, TestJeitaRegion) {
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);
    etl::array<byte, kReadRegisterCount> read_buffer;

    uint16_t Jeita_region = 3;
    double Vcharge = 3.6;
    double Icharge = 1.0;

    CHECK_EQUAL(Jeita_region, bms.GetJeitaRegionVCharge(read_buffer));
    DOUBLES_EQUAL(Vcharge, bms.GetVChargeDEC(read_buffer), 0.5);
    DOUBLES_EQUAL(Icharge, bms.GetIChargeDEC(read_buffer), 0.5);
}
