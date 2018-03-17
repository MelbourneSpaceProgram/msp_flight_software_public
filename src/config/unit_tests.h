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

// This define controls the conditional compilation of some array arguments.
// It adds arguments into the middle of an array, therefore we are unable to use
// the if(var) approach without significant rework. The region of effect of this
// define is also small enough that we feel comfortable keeping it. It should
// not be used in the future without further discussion.
//#define SATELLITE_CONFIG 1
#define LAUNCHPAD_CONFIG 2

extern bool satellite_configuration;
extern bool spi_test_enabled;
extern bool eeprom_test_enabled;
extern bool i2c_test_enabled;
extern bool bms_test_enabled;
extern bool antenna_test_enabled;
extern bool lithium_test_enabled;
extern bool lithium_flash_test_enabled;
extern bool rtc_test_enabled;
extern bool multiplexer_test_enabled;
extern bool adc_test_enabled;
extern bool imu_test_enabled;
extern bool hil_enabled;
extern bool memory_troubleshooter_test_enabled;
extern bool i2c_enumeration_enabled;
#endif  //  SRC_CONFIG_UNIT_TESTS_H_
