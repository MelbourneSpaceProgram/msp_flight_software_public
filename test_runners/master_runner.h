/* AUTOGENERATED FILE. DO NOT EDIT. */

#ifndef TEST_RUNNERS_MASTER_RUNNER_H_
#define TEST_RUNNERS_MASTER_RUNNER_H_

void master_tests_thread();
int b_dot_controller_tests_runner();
int b_dot_estimator_tests_runner();
int kalman_filter_tests_runner();
int location_estimator_tests_runner();
int nadir_controller_tests_runner();
int nadir_error_generator_tests_runner();
int sgp4_tests_runner();
int wmm_tests_runner();
int database_tests_runner();
int runnable_data_dashboard_tests_runner();
int debug_interface_tests_runner();
int i2c_tests_runner();
int integration_i2c_tests_runner();
int bms_tests_runner();
int multiplexer_tests_runner();
int message_tests_runner();
int payload_processor_tests_runner();
int generic_sensor_tests_runner();
int measurable_manager_tests_runner();
int rtc_tests_runner();
int i2c_sensor_tests_runner();
int mpu9250_motion_tracker_tests_runner();
int magnetometer_tests_runner();
int spi_tests_runner();
int sensor_state_logic_tests_runner();
int state_manager_tests_runner();
int system_state_logic_tests_runner();
int antenna_tests_runner();
int lithium_ack_tests_runner();
int lithium_command_serialise_tests_runner();
int lithium_configuration_tests_runner();
int lithium_hardware_tests_runner();
int finite_difference_symmetric_tests_runner();
int first_order_iir_lowpass_tests_runner();
int matrix_tests_runner();
int memory_troubleshooter_tests_runner();
void SetUp();
void TearDown();

#endif  // TEST_RUNNERS_MASTER_RUNNER_H_
