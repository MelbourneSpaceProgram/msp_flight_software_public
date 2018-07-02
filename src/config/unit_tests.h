#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

#include <stdbool.h>
/*

   Usage

   To ignore a Unity test, add the following to the beginning of the test
   method in [name_of_functionality]_tests.cpp:

   if(![name_of_functionality]_test_enabled){
       TEST_IGNORE_MESSAGE("some informative message")
   }

   It is also necessary add the following to this file:

   extern bool [name_of_functionality]_test_enabled;

   And the following to unit_tests.cpp:

   bool [name_of_functionality]_test_enabled = false;

   Please choose a default value that will not break the tests for others using
   a LaunchPad without the necessary hardware attached.

 */

extern bool satellite_configuration;
extern bool spi_test_enabled;
extern bool eeprom_test_enabled;
extern bool i2c_test_enabled;
extern bool lithium_test_enabled;
extern bool lithium_flash_test_enabled;
extern bool adc_test_enabled;
extern bool i2c_enumeration_enabled;
extern bool measurable_test_enabled;
extern bool i2c_enabled;
extern bool sd_test_enabled;
extern bool over_the_air_enabled;
extern bool write_back_test_enabled;
extern bool current_sensor_test_enabled;
extern bool force_reset_command_test_enabled;
extern bool deployment_waits_are_instant;
extern bool i2c_available;
extern bool spi_available;
extern bool sd_card_available;
extern bool lithium_available;
extern bool hil_available;
extern bool imu_available;
extern bool bms_available;
extern bool antenna_available;
extern bool ditl_mode;

#endif  //  SRC_CONFIG_UNIT_TESTS_H_
