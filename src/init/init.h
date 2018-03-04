#ifndef SRC_INIT_INIT_H_
#define SRC_INIT_INIT_H_

#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

// TODO(dingbenjamin): Remove, along with applications reliant on this define
#define TASKSTACKSIZE 8096
Task_Struct allocatorTaskStruct;
Char allocatorTaskStack[TASKSTACKSIZE];

void PreBiosInit();
void PostBiosInit();
void TestInit();

#endif  // SRC_INIT_INIT_H_
