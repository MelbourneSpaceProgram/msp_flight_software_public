/**
 * @file
 */

#include <src/public_headers/init/init.hpp>
#include <xdc/runtime/Log.h>
#include <unistd.h>
#include <stdint.h>
#include <ti/sysbios/posix/pthread.h>
#include <stddef.h>
#include <src/CDH/diagnostics/logger_p.h>
#include <xdc/runtime/System.h>

#include <ti/drivers/UART.h>

#include <ti/sysbios/hal/Hwi.h>

#include "Board.h"
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <src/pwm/pwm.hpp>
#include <src/uart/uart_configuration.hpp>
#include <src/uart/uart.hpp>

#define THREADSTACKSIZE    1024

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
    Board_initPWM();
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
    // TODO Reenable AK
    /*UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        while (1);
    }

    UartLog_init(uart);
    */
}


void uartReceiveFinished(UART_Handle handle, void *uart_rxBuffer, size_t count)
{
    GPIO_toggle(Board_GPIO_LED1);
}

int keep_writing = 1;
// Open the UART stream
    UARTConfiguration uart_config(UARTConfiguration::BLOCKING_MODE, UARTConfiguration::HIGH, &uartReceiveFinished);
    UART uart_bus(&uart_config, Board_UART0);
void * test_task(void *){

    /*double duty_cycle = 0;
    while(1){
        MSP_PWM_set_duty(0, duty_cycle);
        duty_cycle += 0.000001;

        if (duty_cycle > 0.2){
            duty_cycle = 0;
        }
    }*/

    // Test task for reading back to UART


    uart_bus.open();

    int i = 0;
    while(1){
        if (keep_writing){
            char input = 97 + (i % 26);
            int write_buffer_length = 1;
            uart_bus.perform_write_transaction(&input, write_buffer_length);
            i++;
        }
    }
}

void * test_reader(void *){
    char input;
    while(1){
    // This is a non blocking read
        UART_read(uart_bus.get_handle(), &input, 1);
        if (keep_writing){
            keep_writing = 0;
        } else {
            keep_writing = 1;
        }
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

}

void init_satellite1()
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

    retc = pthread_create(&thread, &attrs, test_reader, NULL);
        if (retc != 0) {
            /* pthread_create() failed */
            while (1);
        }
}
