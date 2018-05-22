#include <src/config/unit_tests.h>

bool satellite_configuration = SATELLITE_CONFIG;

bool spi_test_enabled = true;

// Need to be kept disabled for now as they do not initialise an internal
// semaphore when the hardware is not present. This causes it to pend on the
// semaphore eternally.
bool eeprom_test_enabled = false;
bool sd_test_enabled = false;

bool i2c_test_enabled = true;
bool bms_test_enabled = true;
bool antenna_test_enabled = true;
bool lithium_flash_test_enabled = true;
bool lithium_test_enabled = true;
bool rtc_test_enabled = true;
bool multiplexer_test_enabled = true;
bool adc_test_enabled = true;
bool imu_test_enabled = true;
bool hil_enabled = true;
bool memory_troubleshooter_test_enabled = true;
bool i2c_enumeration_enabled = true;
bool measurable_test_enabled = true;

bool write_back_test_enabled = true;
bool i2c_enabled = true;
bool io_expander_test_enabled = true;
bool over_the_air_enabled = true;
bool current_sensor_test_enabled = true;
bool force_reset_command_test_enabled = false;
