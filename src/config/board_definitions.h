#ifndef SRC_CONFIG_BOARD_DEFINITIONS_H_
#define SRC_CONFIG_BOARD_DEFINITIONS_H_

#include <ti/drivers/ADC.h>
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Watchdog.h>

#include <src/board/board.h>

#define Board_initGeneral MSP_EXP432P401R_initGeneral

#define UARTA0 MSP_EXP432P401R_UARTA0
#define UARTA2 MSP_EXP432P401R_UARTA2

#endif  // SRC_CONFIG_BOARD_DEFINITIONS_H_
