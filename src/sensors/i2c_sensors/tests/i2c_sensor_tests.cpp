#include <CppUTest/TestHarness.h>
#include <src/board/board.h>
#include <src/board/i2c/i2c.h>
#include <src/board/i2c/multiplexers/i2c_multiplexer.h>
#include <src/config/unit_tests.h>
#include <src/observers/specific_observers/test_observer.h>
#include <src/sensors/i2c_sensors/adc.h>

// Address value must be changed for each ADC.
static const byte test_adc_address = 0x48;
static const byte kMultiplexerAddress = 0x76;

TEST_GROUP(Adc) {
    void setup() {
        if (!i2c_available || !fs_board_available) {
            TEST_EXIT;
        }
    };
};

TEST(Adc, TestADC) {
    I2c test_i2c_bus(I2C_BUS_A);
    I2cMultiplexer multiplexer(&test_i2c_bus, kMultiplexerAddress);
    multiplexer.OpenChannel(I2cMultiplexer::kMuxChannel1);
    Adc adc(&test_i2c_bus, test_adc_address);

    adc.SetOperationalStatus(kAdcConversion);
    adc.SetOperatingMode(kAdcContinuousConversion);
    double read_voltage = adc.TakeI2cReading();

    multiplexer.CloseAllChannels();

    // Check that the reading is within +-full_scale_range
    DOUBLES_EQUAL(0, read_voltage, adc.GetAdcGainAmplifierFullScaleRange());
}
