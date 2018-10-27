#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

// Console configuration (text output to CCS or Data Dashboard)
// Please look in console_config.txt
static constexpr bool kVerboseUnitTests = false;

static constexpr bool kEpsBoardAvailable = true;
static constexpr bool kCdhBoardAvailable = true;
static constexpr bool kFsBoardAvailable = true;
static constexpr bool kTcomBoardAvailable = true;
static constexpr bool kUtilitiesBoardAvailable = true;

static constexpr bool kXPosAvailable = true;
static constexpr bool kXNegAvailable = true;
static constexpr bool kYPosAvailable = true;
static constexpr bool kYNegAvailable = true;
static constexpr bool kZPosAvailable = true;
static constexpr bool kZNegAvailable = true;

static constexpr bool kLithiumFlashTestEnabled = false;
static constexpr bool kForceResetCommandEnabled =
    false;  // It will reset the satellite

// Hardware available
static constexpr bool kEepromAvailable =
    false;  // It is actually available but there is a resource conflict with
            // the SD cards as they share the same bus
static constexpr bool kI2cAvailable = true;
static constexpr bool kSdCardAvailable = true;
static constexpr bool kHilAvailable = false;
static constexpr bool kBmsAvailable = true;
static constexpr bool kAntennaAvailable = false;
static constexpr bool kDitlMode = false;

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

static const char* test_args[] = {"exe", "-sg", "Bms"};

#endif  //  SRC_CONFIG_UNIT_TESTS_H_
