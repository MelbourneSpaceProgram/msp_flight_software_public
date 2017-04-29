/*
 * #include <ti/drivers/UART.h>
#include "Board.h"

#include <src/Telecomms/init_p.hpp>
#include <src/public_headers/logger.h>
UART_Handle uart;
UART_Params uartParams;

void init_telecomms(){
    // TODO Perhaps we need to bring state in here?
    // Or flags to enable/disable certain subsystems
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.writeMode = UART_MODE_CALLBACK;
    uartParams.writeCallback = UART_Callbk_read;
    uart = UART_open(Board_UART0, &uartParams);
}

void UART_Callbk_read (UART_Handle handle, void *buffer, int size){

void * readB = buffer;


//log_info(readB);

}
*/
