#ifndef MSP_INIT_HPP
#define MSP_INIT_HPP

#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SDSPI.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>
#include <ti/display/Display.h>

#include "Board.h"

#include <src/public_headers/time.hpp>
#include <src/public_headers/diagnostics.hpp>
#include <src/public_headers/allocator.hpp>

#define TASKSTACKSIZE   8096
Task_Struct allocatorTaskStruct;
Char allocatorTaskStack[TASKSTACKSIZE];

void init_core();
void init_time();
void init_logger();
void init_diagnostics();
void init_satellite();

#endif
