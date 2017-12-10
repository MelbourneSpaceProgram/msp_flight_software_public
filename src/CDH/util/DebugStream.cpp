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
    char input = 0x0A;
    //debug.perform_write_transaction(& (char) (serial_msg.size), 1); // TODO Check that the size is always one byte
    debug.perform_write_transaction(serial_msg.buffer, serial_msg.size);
    //debug.perform_write_transaction(&input, 1);
}

