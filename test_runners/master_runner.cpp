/* AUTOGENERATED FILE. DO NOT EDIT. */

#include <test_runners/database_tests.h>
#include <test_runners/debug_interface_tests.h>
#include <test_runners/i2c_tests.h>
#include <test_runners/multiplexer_tests.h>
#include <test_runners/message_tests.h>
#include <test_runners/generic_sensor_tests.h>
#include <test_runners/rtc_tests.h>
#include <test_runners/i2c_sensor_tests.h>
#include <test_runners/spi_tests.h>
#include <test_runners/sensor_state_logic_tests.h>
#include <test_runners/state_manager_tests.h>
#include <test_runners/system_state_logic_tests.h>
#include <test_runners/antenna_tests.h>
#include <test_runners/lithium_ack_tests.h>
#include <test_runners/lithium_command_serialise_tests.h>
#include <test_runners/lithium_configuration_tests.h>
#include <test_runners/lithium_hardware_tests.h>
#include <test_runners/finite_difference_symmetric_tests.h>
#include <test_runners/first_order_iir_lowpass_tests.h>
#include <test_runners/matrix_tests.h>
#include <test_runners/master_runner.h>
void master_tests_thread() {
    database_tests_runner();
    debug_interface_tests_runner();
    i2c_tests_runner();
    multiplexer_tests_runner();
    message_tests_runner();
    generic_sensor_tests_runner();
    rtc_tests_runner();
    i2c_sensor_tests_runner();
    spi_tests_runner();
    sensor_state_logic_tests_runner();
    state_manager_tests_runner();
    system_state_logic_tests_runner();
    antenna_tests_runner();
    lithium_ack_tests_runner();
    lithium_command_serialise_tests_runner();
    lithium_configuration_tests_runner();
    lithium_hardware_tests_runner();
    finite_difference_symmetric_tests_runner();
    first_order_iir_lowpass_tests_runner();
    matrix_tests_runner();
}


void SetUp() {}
void TearDown() {}
