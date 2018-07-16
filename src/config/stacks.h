#ifndef SRC_CONFIG_STACKS_H_
#define SRC_CONFIG_STACKS_H_

#include <src/util/data_types.h>

constexpr uint16_t radio_listener_stack_size = 1200;
constexpr uint16_t unit_tests_stack_size = 20000;
constexpr uint16_t beacon_stack_size = 1536;
constexpr uint16_t memory_logger_stack_size = 1024;
constexpr uint16_t time_source_stack_size = 1500;

#endif  //  SRC_CONFIG_STACKS_H_
