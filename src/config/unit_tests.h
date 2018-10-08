#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

// Console configuration (text output to CCS or Data Dashboard)
// Please look in console_config.txt
static const bool kVerboseUnitTests = false;

static const bool kEpsBoardAvailable = true;
static const bool kCdhBoardAvailable = true;
static const bool kFsBoardAvailable = true;
static const bool kTcomBoardAvailable = true;
static const bool kUtilitiesBoardAvailable = true;

static const bool kXPosAvailable = true;
static const bool kXNegAvailable = true;
static const bool kYPosAvailable = true;
static const bool kYNegAvailable = true;
static const bool kZPosAvailable = true;
static const bool kZNegAvailable = true;

// TODO(akremor): Begin deprecating these once we are comfortable with hardware
// stability
static const bool kLithiumFlashTestEnabled = false;
static const bool kForceResetCommandEnabled =
    false;  // It will reset the satellite

// Hardware available
static const bool kEepromAvailable =
    false;  // It is actually available but there is a resource conflict with
            // the SD cards as they share the same bus
static const bool kI2cAvailable = true;
static const bool kSdCardAvailable = true;
static const bool kHilAvailable = false;
static const bool kBmsAvailable = true;
static const bool kAntennaAvailable = false;
static const bool kDitlMode = false;

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
