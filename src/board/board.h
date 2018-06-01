#ifndef __BOARD_H
#define __BOARD_H

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

#include "MSP_EXP432P401R.h"

#define Board_initGeneral MSP_EXP432P401R_initGeneral

#define Board_TIMER2 MSP_EXP432P401R_TIMER_TA_1
#define Board_TIMER3 MSP_EXP432P401R_TIMER_TA_2

#endif /* __BOARD_H */
