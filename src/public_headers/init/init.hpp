#ifndef MSP_INIT_HPP
#define MSP_INIT_HPP

#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

#include "../SatelliteTime.hpp"

#include <src/pwm/pwm.hpp>

#define TASKSTACKSIZE   8096
Task_Struct allocatorTaskStruct;
Char allocatorTaskStack[TASKSTACKSIZE];

void init_core();
void init_time();
void init_logger();
void init_satellite();
void init_satellite1();

#endif
