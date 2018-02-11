#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

/*

   Usage

   To ignore a Unity test, add the following to the beginning of the test
   method in [name_of_functionality]_tests.cpp:

   #ifdef [name_of_functionality]_TESTS_IGNORED
       TEST_IGNORE_MESSAGE("some informative message")
   #endif

   It is also necessary add the following to this file:

   #define [name_of_functionality]_TESTS_IGNORED

   By default, hardware tests are ignored.

 */

#define I2C_TESTS_IGNORED
#define ANTENNA_TESTS_IGNORED
#define LITHIUM_TESTS_IGNORED

#endif  //  SRC_CONFIG_UNIT_TESTS_H_
