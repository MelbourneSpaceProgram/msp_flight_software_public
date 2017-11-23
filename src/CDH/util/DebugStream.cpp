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

DebugStream::DebugStream()
{
    char        input;
    char  echoPrompt[] = "Debug stream started.\r\n";

    /* Call driver init functions */
    GPIO_init();
    UART_init();

    debugConfig = UARTConfiguration(UARTConfiguration::BLOCKING_MODE, UARTConfiguration::BAUD_115200);
    debug = UART(&debugConfig, UARTA0);

    debug.perform_write_transaction(echoPrompt, sizeof(echoPrompt));
}

void DebugStream::handleMessage( TemperatureMessage *message )
  {
    char        input[] = "Received debug message.\r\n";
    char msg = (char) message->timestamp;
    debug.perform_write_transaction(input, sizeof(input));

    debug.perform_write_transaction(&msg, 1);
    int noop = 123;
  }

DebugStream::~DebugStream()
{

    // TODO Auto-generated destructor stub
}

