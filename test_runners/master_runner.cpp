/* AUTOGENERATED FILE. DO NOT EDIT. */

#include <test_runners/debug_interface_tests.h>
#include <test_runners/message_tests.h>
#include <test_runners/master_runner.h>
void master_tests_thread() {
    debug_interface_tests_runner();
    message_tests_runner();
}


void SetUp() {}
void TearDown() {}
