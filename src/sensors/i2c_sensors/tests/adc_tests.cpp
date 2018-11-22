#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <src/util/msp_exception.h>

// Address value must be changed for each ADC.
static constexpr byte test_adc_address = 0x48;
static constexpr byte kMultiplexerAddress = 0x76;

TEST_GROUP(Adc) {
    void setup() {
        if (!kI2cAvailable || !kFsBoardAvailable) {
            TEST_EXIT;
        }
    };
};

TEST(Adc, TestADC) {
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    Adc adc(&test_i2c_bus, test_adc_address, &multiplexer,
            I2cMultiplexer::kMuxChannel2);

    adc.SetOperationalStatus(kAdcConversion);
    adc.SetOperatingMode(kAdcContinuousConversion);
    double read_voltage = -9999;

    try {
        read_voltage = adc.TakeI2cReading();
    } catch (MspException& e) {
        FAIL("Exception occurred in TakeI2cReading");
    }

    // Check that the reading is within +-full_scale_range
    DOUBLES_EQUAL(0, read_voltage, adc.GetAdcGainAmplifierFullScaleRange());
}
