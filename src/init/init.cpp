/**
 * @file
 */

#include <src/public_headers/init/init.hpp>
#include <xdc/runtime/Log.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <src/CDH/diagnostics/logger_p.h>
#include <xdc/runtime/System.h>


#include <ti/drivers/UART.h>

#include <ti/sysbios/hal/Hwi.h>

#include <xdc/runtime/Log.h>
#include <xdc/runtime/Text.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>
#include <ti/display/Display.h>

#include "Board.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

UART_Handle uart = NULL;

/**
 * Initialises the core MSP432 drivers provided by TI. Should be called once at system startup, and prior to the BIOS starting.
 */
void init_core()
{
    Board_initGeneral();
    Board_initGPIO();
    Board_initI2C();
    Board_initSPI();
    Board_initUART();
}

/**
 * Initialises the system time. The default epoch is 2017/01/01 00:00 UTC. The actual system time can be set via the SatelliteTiming module.
 */
void init_time()
{
    // Set the current time to 2017/01/01 00:00 UTC
    SatelliteTime::set_utc_time(((uint64_t) 1483228800000));
}

/**
 * Initialises the logging output of the satellite. Not yet written.
 */
void init_logger()
{
    UART_Params uartParams;

    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }

    UartLog_init(uart);
}

void test_time(){
    while(1){
        Log_info0("I am a log");


        Task_sleep(1000);
        Log_error0("I am an error");
        Log_error1("I am an error with a parameter: %d", 123);
        uint64_t curr = SatelliteTime::get_utc_time();
        //System_printf("%hu\n", curr);
        //System_flush();
        //Task_sleep(1);
    }
}
//int _tmain();

/**
 * Initialises the diagnostic output of the satellite. Currently this is just the heartbeat LED, although it may be expanded to include self-tests.
 */
void init_diagnostics()
{
    initHeartbeat();
}

/**
 * Starts the main process running on the satellite, the task scheduler loop. Should only be called once all other initialisation tasks have completed successfully.
 */
void init_satellite()
{
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.instance->name = "allocator_request";
    taskParams.stack = &allocatorTaskStack;
    taskParams.priority = 2;
    taskParams.arg0 = 1;

    Task_construct(&allocatorTaskStruct, (Task_FuncPtr) test_time,
                   &taskParams, NULL);
    Task_Handle task = Task_handle(&allocatorTaskStruct);
}
