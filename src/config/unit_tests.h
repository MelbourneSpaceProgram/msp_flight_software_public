#ifndef SRC_CONFIG_UNIT_TESTS_H_
#define SRC_CONFIG_UNIT_TESTS_H_

static const bool eps_board_available = false;
static const bool cdh_board_available = true;
static const bool fs_board_available = false;
static const bool panel_board_available = false;
static const bool tcom_board_available = false;
static const bool utilities_board_available = false;

// TODO(akremor): Begin deprecating these once we are comfortable with hardware
// stability
static const bool lithium_flash_test_enabled = false;
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
static const bool sd_card_available = false;
static const bool hil_available = false;
static const bool bms_available = false;
static const bool antenna_available = false;
static const bool ditl_mode = false;

static const bool log_i2c = false;

#endif  //  SRC_CONFIG_UNIT_TESTS_H_
