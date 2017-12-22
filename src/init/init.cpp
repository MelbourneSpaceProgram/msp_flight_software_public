/**
 * @file
 */

//#include <src/config/board_definitions.h>
#include "Board.h"
#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2
#include <pthread.h>
#include <src/public_headers/init/init.hpp>
#include <src/tasks/tasks.hpp>

#define THREADSTACKSIZE 1024

/**
 * Initialises the core MSP432 drivers provided by TI. Should be called once at
 * system startup, and prior to the BIOS starting.
 */
void init_core() {
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
 * Initialises the system time. The default epoch is 2017/01/01 00:00 UTC. The
 * actual system time can be set via the SatelliteTiming module.
 */
void init_time() {
    // Set the current time to 2017/01/01 00:00 UTC
    // SatelliteTime::set_utc_time(((uint64_t) 1483228800000));
}

void *test_task(void *) {
    while (1) {
        Task_sleep(500);
    }
}

/**
 * Starts the main process running on the satellite, the task scheduler loop.
 * Should only be called once all other initialisation tasks have completed
 * successfully.
 */
void init_satellite() {
    pthread_t thread;
    pthread_attr_t attrs;
    struct sched_param priParam;
    int retc;
    int detachState;

    pthread_attr_init(&attrs);
    priParam.sched_priority = 8;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1)
            ;
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1)
            ;
    }

    retc = pthread_create(&thread, &attrs, test_task, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1)
            ;
    }

    InitTasks();
}
