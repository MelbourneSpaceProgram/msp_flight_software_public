#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

#include <stdbool.h>

static const bool lithium_flash_test_enabled = false;
static const bool measurable_test_enabled = false;
static const bool i2c_enabled = true;  // TODO Shift to I2c available
static const bool over_the_air_enabled =
    false;  // Lithium not enabled. Rename to be lithium_available? TODO
static const bool write_back_test_enabled = true;
static const bool current_sensor_test_enabled =
    false;  // No current sensors on CDH alone TODO
static const bool force_reset_command_test_enabled =
    false;  // It will reset the satellite
static const bool deployment_waits_are_instant = true;

// Hardware available
static const bool eeprom_available =
    false;  // It is actually available but there is a resource conflict with
            // the SD cards as they share the same bus
static const bool i2c_available = true;
static const bool spi_available = false;
static const bool sd_card_available = true;
static const bool lithium_available = false;  // Lithium not responding?
static const bool hil_available = false;
static const bool imu_available =
    false;  // Causes a crash when reading the magnetometer adjust registers
static const bool bms_available = false;  // TODO Crashed
static const bool antenna_available = false;
static const bool ditl_mode = false;

static const bool enumerate_i2c_devices = false;
static const bool log_i2c = false;

#endif  //  SRC_CONFIG_UNIT_TESTS_H_
