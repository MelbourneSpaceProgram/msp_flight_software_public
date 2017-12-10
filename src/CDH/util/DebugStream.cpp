/*
 * DebugStream.cpp
 *
 *  Created on: 19Nov.,2017
 *      Author: Anthony
 */

#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>

#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

#include <src/CDH/util/DebugStream.h>

DebugStream::DebugStream() {
    char echo_prompt[] = "Debug stream started.\r\n";

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    debug_config = UARTConfiguration(UARTConfiguration::BLOCKING_MODE, UARTConfiguration::BAUD_115200);
    debug = UART(&debug_config, UARTA0);

    debug.perform_write_transaction(echo_prompt, sizeof(echo_prompt));
}

DebugStream::~DebugStream() {
    // TODO Auto-generated destructor stub
}

void DebugStream::SendMessage(SerialisedMessage serial_msg) {
    debug.perform_write_transaction(serial_msg.buffer, serial_msg.size);
}

uint8_t DebugStream::ReceiveCode() {
    char read_code[1];
    uint8_t read_code_length = 1;
    debug.perform_read_transaction(read_code, read_code_length);
    return (uint8_t)read_code;
}

