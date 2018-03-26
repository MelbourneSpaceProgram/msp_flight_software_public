#include <Board.h>
#include <src/config/unit_tests.h>
#include <src/i2c/i2c.h>
#include <src/observers/specific_observers/test_observer.h>
#include <src/sensors/i2c_sensors/adc.h>
#include <test_runners/i2c_sensor_tests.h>
#include <test_runners/unity.h>

// Address value must be changed for each ADC.
static const byte test_adc_address = 0x48;

void TestADC(void) {
    if (!adc_test_enabled) {
        TEST_IGNORE_MESSAGE("Hardware test ignored");
    }
    I2c test_i2c_bus(Board_I2C_TMP);
    Adc adc(&test_i2c_bus, test_adc_address);

    adc.SetOperationalStatus(kAdcConversion);
    adc.SetOperatingMode(kAdcContinuousConversion);
    adc.TakeReading();
    double read_voltage = adc.GetReading();

    // Check that the reading is within +-full_scale_range
    TEST_ASSERT_DOUBLE_WITHIN(2 * adc.GetAdcGainAmplifierFullScaleRange(),
                              adc.GetAdcGainAmplifierFullScaleRange(),
                              adc.GetReading());
    TEST_ASSERT_DOUBLE_WITHIN(2 * adc.GetAdcGainAmplifierFullScaleRange(),
                              -adc.GetAdcGainAmplifierFullScaleRange(),
                              adc.GetReading());
}
