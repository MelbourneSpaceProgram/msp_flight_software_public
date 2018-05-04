#include <Board.h>
#include <external/etl/array.h>
#include <src/config/unit_tests.h>
#include <src/i2c/bms/bms.h>
#include <src/i2c/i2c.h>
#include <src/sensors/i2c_measurable_manager.h>
#include <test_runners/bms_tests.h>
#include <test_runners/unity.h>
#include <ti/sysbios/knl/Task.h>

// Address value must be changed for each BMS.
static const byte test_bms_address = 0x68;
static const uint8_t kReadRegisterCount = 2;

void TestBms()
{
    if (!bms_test_enabled)
    {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }

    // (SCL, SDA) -> (3.5,3.6)
    I2c bus_d(I2C_BUS_D);
    Bms bms(&bus_d, test_bms_address);
    etl::array<byte, kReadRegisterCount> read_buffer;

    TEST_ASSERT_EQUAL_INT16(Bms::kUVCLRegisterValue,
                            bms.GetConfiguration(0x16, read_buffer));
    TEST_ASSERT_EQUAL_INT16(Bms::kVChargeRegisterValue,
                            bms.GetConfiguration(0x1B, read_buffer));
    TEST_ASSERT_EQUAL_INT16(Bms::kIChargeRegisterValue,
                            bms.GetConfiguration(0x1A, read_buffer));
    TEST_ASSERT_EQUAL_INT16(
            (Bms::kReChargeThresholdURegisterValue << 8
                    | Bms::kReChargeThresholdLRegisterValue),
            bms.GetConfiguration(0x2E, read_buffer));

    TEST_ASSERT_NOT_EQUAL(Bms::kError, bms.GetChargeStatus(read_buffer));
    TEST_ASSERT_NOT_EQUAL(Bms::kOther, bms.GetSystemStatus(read_buffer));
}

void TestBmsDieTemperatureRead()
{
    if (!bms_test_enabled)
    {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2cMeasurableManager* i2c_measurable_manager =
            I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(
            kPowerBmsDieTemp1, 1);
    TEST_ASSERT_GREATER_THAN(0, temp);
}

void TestBmsBatteryTemperatureRead()
{
    if (!bms_test_enabled)
    {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2cMeasurableManager* i2c_measurable_manager =
            I2cMeasurableManager::GetInstance();
    double temp = i2c_measurable_manager->ReadI2cMeasurable<double>(
            kPowerBmsBatteryTemp1, 1);
    TEST_ASSERT_GREATER_THAN(0, temp);
}
