#include <src/config/unit_tests.h>


bool spi_test_enabled = false;

// Need to be kept disabled for now as they do not initialise an internal
// semaphore when the hardware is not present. This causes it to pend on the
// semaphore eternally.
bool eeprom_test_enabled = false;
bool sd_test_enabled = false;

bool i2c_test_enabled = true;
bool lithium_flash_test_enabled = true;
bool lithium_test_enabled = true;
bool adc_test_enabled = true;
bool i2c_enumeration_enabled = false;
bool measurable_test_enabled = true;
bool write_back_test_enabled = true;
bool i2c_enabled = true;
bool over_the_air_enabled = true;
bool current_sensor_test_enabled = false;
bool force_reset_command_test_enabled = false;

bool i2c_available = true;
bool spi_available = false;  // TODO Crashed
bool sd_card_available = false;
bool lithium_available =
    false;  // TODO Tests failing because Lithium not plugged in?
bool imu_available = false; // Bad cabling
bool hil_available = false;
bool bms_available = false;  // TODO Crashed
bool antenna_available = false;
bool ditl_mode = false;

bool deployment_waits_are_instant = true;
