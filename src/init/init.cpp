/**
 * @file
 */

//#include <src/config/board_definitions.h>
#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2
#include <src/public_headers/init/init.hpp>
#include <src/tasks/tasks.hpp>
#include <src/isr/isr.hpp>
#include <ti/sysbios/posix/pthread.h>
// Logging related
#include <xdc/runtime/Log.h>
#include <ti/drivers/UART.h>
#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>
#include <src/CDH/diagnostics/logger_p.h>


#define THREADSTACKSIZE    1024

// TODO Move this UART handle into the global state object
UART_Handle uart = NULL;

/**
 * Initialises the core MSP432 drivers provided by TI. Should be called once at system startup, and prior to the BIOS starting.
 */
void init_core()
{
    // TODO Refactor this to use style below
    Board_initGeneral();
    GPIO_init();
    UART_init();
    I2C_init();
    SPI_init();

    UART_init();
    PWM_init();

    MSP_pwm_init();
}



/**
 * Initialises the system time. The default epoch is 2017/01/01 00:00 UTC. The actual system time can be set via the SatelliteTiming module.
 */
void init_time()
{
    // Set the current time to 2017/01/01 00:00 UTC
    //SatelliteTime::set_utc_time(((uint64_t) 1483228800000));
}


/**
 * Initialises the logging output of the satellite. Not yet written.
 */
void init_logger()
{

    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(UARTA2, &uartParams);

    if (uart == NULL) {
        while (1);
    }

    UartLog_init(uart);
}



void * test_task(void *){
    while(1){
        Task_sleep(500);
    }
}

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
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    pthread_attr_init(&attrs);
    priParam.sched_priority = 8;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    retc = pthread_create(&thread, &attrs, test_task, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }
    
    InitTasks();
    InitInterrupts();
}

