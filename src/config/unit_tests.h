#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

static const bool eps_board_available = true;
static const bool cdh_board_available = true;
static const bool fs_board_available = true;
static const bool panel_board_available = true;
static const bool tcom_board_available = true;
static const bool utilities_board_available = true;

// TODO(akremor): Begin deprecating these once we are comfortable with hardware
// stability
static const bool lithium_flash_test_enabled = false;
static const bool current_sensor_test_enabled =
    false;  // No current sensors on CDH alone TODO
static const bool force_reset_command_test_enabled =
    false;  // It will reset the satellite

// Hardware available
static const bool eeprom_available =
    false;  // It is actually available but there is a resource conflict with
            // the SD cards as they share the same bus
static const bool i2c_available = true;
static const bool sd_card_available = true;
static const bool hil_available = false;
static const bool bms_available = false;
static const bool antenna_available = false;
static const bool ditl_mode = false;

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
