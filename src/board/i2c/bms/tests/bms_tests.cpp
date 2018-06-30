#include <external/etl/array.h>
#include <src/board/board.h>
#include <src/board/i2c/bms/bms.h>
#include <src/board/i2c/i2c.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <test_runners/unity.h>

// Address value must be changed for each BMS.
static const byte test_bms_address = 0x68;
static const uint8_t kReadRegisterCount = 2;

void TestBms() {
    if (!bms_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }

    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);
    etl::array<byte, kReadRegisterCount> read_buffer;

    TEST_ASSERT_DOUBLE_WITHIN(1.0, 3.0, bms.GetBatVoltage(read_buffer));
    TEST_ASSERT_DOUBLE_WITHIN(1.0, 1.0, bms.GetBatCurrent(read_buffer));
    TEST_ASSERT_DOUBLE_WITHIN(3.0, 3.0, bms.GetInVoltage(read_buffer));
    TEST_ASSERT_DOUBLE_WITHIN(0.5, 0.5, bms.GetInCurrent(read_buffer));
    TEST_ASSERT_DOUBLE_WITHIN(2.0, 4.0, bms.GetSysVoltage(read_buffer));

    TEST_ASSERT_EQUAL_DOUBLE(3.65, bms.GetVChargeDEC(read_buffer));
    TEST_ASSERT_EQUAL_DOUBLE(1.0, bms.GetIChargeDEC(read_buffer));

    TEST_ASSERT_EQUAL_INT16((Bms::kUVCLRegisterUValue << 8 |
                             Bms::kUVCLRegisterLValue),
                            bms.GetConfiguration(0x16, read_buffer));
    TEST_ASSERT_EQUAL_INT16((Bms::kReChargeThresholdURegisterValue << 8 |
                             Bms::kReChargeThresholdLRegisterValue),
                            bms.GetConfiguration(0x2E, read_buffer));
    TEST_ASSERT_EQUAL_INT16((Bms::kCXThresholdRegisterUValue << 8 |
                             Bms::kCXThresholdRegisterLValue),
                            bms.GetConfiguration(0x1C, read_buffer));

    TEST_ASSERT_NOT_EQUAL(Bms::kError, bms.GetChargeStatus(read_buffer));
    TEST_ASSERT_NOT_EQUAL(Bms::kOther, bms.GetSystemStatus(read_buffer));
}

void TestBmsDieTemperatureRead() {
    if (!bms_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp =
        i2c_measurable_manager->ReadI2cMeasurable<double>(kPowerBmsDieTemp1, 1);
    TEST_ASSERT_GREATER_THAN(0, temp);
}

void TestBmsBatteryTemperatureRead() {
    if (!bms_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2cMeasurableManager* i2c_measurable_manager =
        I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(
        kPowerBmsBatteryTemp1, 1);
    TEST_ASSERT_GREATER_THAN(0, temp);
}

void TestJeitaRegion() {
    if (!bms_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }

    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);
    etl::array<byte, kReadRegisterCount> read_buffer;

    uint16_t Jeita_region = 2;
    double Vcharge = 3.65;
    double Icharge = 1.0;

    TEST_ASSERT_EQUAL_INT16(Jeita_region,
                            bms.GetJeitaRegionVCharge(read_buffer));
    TEST_ASSERT_EQUAL_DOUBLE(Vcharge, bms.GetVChargeDEC(read_buffer));
    TEST_ASSERT_EQUAL_DOUBLE(Icharge, bms.GetIChargeDEC(read_buffer));
    TEST_ASSERT_DOUBLE_WITHIN(0.15, Vcharge, bms.GetVChargeDEC(read_buffer));
    TEST_ASSERT_DOUBLE_WITHIN(0.1, Icharge, bms.GetIChargeDEC(read_buffer));
}
