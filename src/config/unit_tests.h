#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

#include <src/config/satellite.h>

// Console configuration (text output to CCS or Data Dashboard)
// Please look in console_config.txt

// Writes to Lithium flash
static constexpr bool kLithiumFlashTestEnabled = false;

// Resets the satellite
static constexpr bool kForceResetTestEnabled = false;

// Resets multiple times during the unit tests
static constexpr bool kResetCounterTestEnabled = false;

// Erases the flash memory
static constexpr bool kEraseFlashTestEnabled = false;

// Resets the satellite
static constexpr bool kTestSetBootStateUplink = false;

// See CppUTest Guide.md in top folder for how-to. The below examples can be
// chained together to run all but some tests, or to only run a certain set
// of tests. A test that has been bypassed due to a configuration in
// src/config/unit_tests.h will report as a success.
// Examples:

// Run all tests quietly, printing only final statistics and any failures
// static const char* test_args[] = {"exe"};

// Run all tests verbosely
// static const char* test_args[] = {"exe", "-v"};

// Run only this group of tests
// static const char* test_args[] = {"exe", "-sg", "NameOfTestGroup"};

// Run everything but this group of tests
// static const char* test_args[] = {"exe", "-xg", "NameOfTestGroup"};

// Run only this test
// static const char* test_args[] = {"exe", "-sn", "NameOfTest"};

// Run everything but this test
// static const char* test_args[] = {"exe", "-xn", "NameOfTest"};

static const char* test_args[] = {"exe"};

#endif  //  SRC_CONFIG_UNIT_TESTS_H_
