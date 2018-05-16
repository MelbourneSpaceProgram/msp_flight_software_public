/* AUTOGENERATED FILE. DO NOT EDIT. */

#include <test_runners/b_dot_controller_tests.h>
#include <test_runners/b_dot_estimator_tests.h>
#include <test_runners/kalman_filter_tests.h>
#include <test_runners/location_estimator_tests.h>
#include <test_runners/nadir_controller_tests.h>
#include <test_runners/nadir_error_generator_tests.h>
#include <test_runners/sgp4_tests.h>
#include <test_runners/wmm_tests.h>
#include <test_runners/circular_buffer_nanopb_tests.h>
#include <test_runners/database_tests.h>
#include <test_runners/sd_tests.h>
#include <test_runners/runnable_data_dashboard_tests.h>
#include <test_runners/debug_interface_tests.h>
#include <test_runners/i2c_tests.h>
#include <test_runners/integration_i2c_tests.h>
#include <test_runners/bms_tests.h>
#include <test_runners/io_expander_tests.h>
#include <test_runners/multiplexer_tests.h>
#include <test_runners/message_tests.h>
#include <test_runners/payload_processor_tests.h>
#include <test_runners/cache_write_back_tests.h>
#include <test_runners/generic_sensor_tests.h>
#include <test_runners/magnetometer_calibration_tests.h>
#include <test_runners/measurable_manager_tests.h>
#include <test_runners/rtc_tests.h>
#include <test_runners/i2c_sensor_tests.h>
#include <test_runners/mpu9250_motion_tracker_tests.h>
#include <test_runners/magnetometer_tests.h>
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
#include <test_runners/memory_troubleshooter_tests.h>
#include <test_runners/master_runner.h>

static const char startMessage[] = "STARTING ON TARGET UNIT TESTS";
static const char endMessage[] = "FINISHED ON TARGET UNIT TESTS";

void master_tests_thread() {

    UNITY_PRINT_EOL();
    UnityPrint(startMessage);
    UNITY_PRINT_EOL();

    b_dot_controller_tests_runner();
    b_dot_estimator_tests_runner();
    location_estimator_tests_runner();
    nadir_controller_tests_runner();
    nadir_error_generator_tests_runner();
    circular_buffer_nanopb_tests_runner();
    database_tests_runner();
    sd_tests_runner();
    runnable_data_dashboard_tests_runner();
    debug_interface_tests_runner();
    i2c_tests_runner();
    integration_i2c_tests_runner();
    bms_tests_runner();
    io_expander_tests_runner();
    multiplexer_tests_runner();
    message_tests_runner();
    payload_processor_tests_runner();
    cache_write_back_tests_runner();
    generic_sensor_tests_runner();
    magnetometer_calibration_tests_runner();
    measurable_manager_tests_runner();
    rtc_tests_runner();
    i2c_sensor_tests_runner();
    mpu9250_motion_tracker_tests_runner();
    magnetometer_tests_runner();
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
    memory_troubleshooter_tests_runner();

    UNITY_PRINT_EOL();
    UnityPrint(endMessage);
    UNITY_PRINT_EOL();
}


void SetUp() {}
void TearDown() {}
