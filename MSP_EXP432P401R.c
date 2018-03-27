/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== MSP_EXP432P401R.c ========
 *  This file is responsible for setting up the board specific items for the
 *  MSP_EXP432P401R board.
 */

#include <src/config/unit_tests.h>
#include <stdbool.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

#include <ti/devices/msp432p4xx/driverlib/dma.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/i2c.h>
#include <ti/devices/msp432p4xx/driverlib/interrupt.h>
#include <ti/devices/msp432p4xx/driverlib/pmap.h>
#include <ti/devices/msp432p4xx/driverlib/ref_a.h>
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/spi.h>
#include <ti/devices/msp432p4xx/driverlib/timer32.h>
#include <ti/devices/msp432p4xx/driverlib/timer_a.h>
#include <ti/devices/msp432p4xx/driverlib/uart.h>
#include <ti/devices/msp432p4xx/driverlib/wdt_a.h>
#include <ti/devices/msp432p4xx/inc/msp.h>

#include "MSP_EXP432P401R.h"

/*
 *  ============================= Capture =============================
 */
#include <ti/drivers/Capture.h>
#include <ti/drivers/capture/CaptureMSP432.h>

CaptureMSP432_Object captureMSP432Objects[MSP_EXP432P401R_CAPTURECOUNT];

const CaptureMSP432_HWAttrs captureMSP432HWAttrs[MSP_EXP432P401R_CAPTURECOUNT] =
    {
        /* Timer_A1 */
        {.timerBaseAddress = TIMER_A1_BASE,
         .clockSource = TIMER_A_CLOCKSOURCE_ACLK,
         .clockDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64,
         .capturePort = CaptureMSP432_P7_7_TA1,
         .intPriority = ~0},
        /* Timer_A2 */
        {.timerBaseAddress = TIMER_A2_BASE,
         .clockSource = TIMER_A_CLOCKSOURCE_ACLK,
         .clockDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64,
         .capturePort = CaptureMSP432_P6_7_TA2,
         .intPriority = ~0},
        /* Timer_A3 */
        {.timerBaseAddress = TIMER_A3_BASE,
         .clockSource = TIMER_A_CLOCKSOURCE_ACLK,
         .clockDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1,
         .capturePort = CaptureMSP432_P8_2_TA3,
         .intPriority = ~0}};

const Capture_Config Capture_config[MSP_EXP432P401R_CAPTURECOUNT] = {
    {.fxnTablePtr = &CaptureMSP432_captureFxnTable,
     .object = &captureMSP432Objects[MSP_EXP432P401R_CAPTURE_TA1],
     .hwAttrs = &captureMSP432HWAttrs[MSP_EXP432P401R_CAPTURE_TA1]},
    {.fxnTablePtr = &CaptureMSP432_captureFxnTable,
     .object = &captureMSP432Objects[MSP_EXP432P401R_CAPTURE_TA2],
     .hwAttrs = &captureMSP432HWAttrs[MSP_EXP432P401R_CAPTURE_TA2]},
    {.fxnTablePtr = &CaptureMSP432_captureFxnTable,
     .object = &captureMSP432Objects[MSP_EXP432P401R_CAPTURE_TA3],
     .hwAttrs = &captureMSP432HWAttrs[MSP_EXP432P401R_CAPTURE_TA3]}};

const uint_least8_t Capture_count = MSP_EXP432P401R_CAPTURECOUNT;

/*
 *  =============================== DMA ===============================
 */
#include <ti/drivers/dma/UDMAMSP432.h>

#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(dmaControlTable, 256)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 256
#elif defined(__GNUC__)
__attribute__((aligned(256)))
#endif
static DMA_ControlTable dmaControlTable[8];

/*
 *  ======== dmaErrorHwi ========
 *  This is the handler for the uDMA error interrupt.
 */
static void dmaErrorHwi(uintptr_t arg) {
    int status = MAP_DMA_getErrorStatus();
    MAP_DMA_clearErrorStatus();

    /* Suppress unused variable warning */
    (void)status;

    while (1)
        ;
}

UDMAMSP432_Object udmaMSP432Object;

const UDMAMSP432_HWAttrs udmaMSP432HWAttrs = {
    .controlBaseAddr = (void *)dmaControlTable,
    .dmaErrorFxn = (UDMAMSP432_ErrorFxn)dmaErrorHwi,
    .intNum = INT_DMA_ERR,
    .intPriority = (~0)};

const UDMAMSP432_Config UDMAMSP432_config = {.object = &udmaMSP432Object,
                                             .hwAttrs = &udmaMSP432HWAttrs};


/*
 *  ======== MSP_EXP432P401R_initGeneral ========
 */
void MSP_EXP432P401R_initGeneral(void) { Power_init(); }

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432.h>

/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP432P401R.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    GPIOMSP432_P9_4 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P9_5 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P1_4 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P2_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P1_5 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P2_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P10_4 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P4_7 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P5_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P5_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P1_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P2_7 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P6_0 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P6_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P6_2 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P6_6 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P6_7 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432_P10_1 | GPIO_CFG_IN_NOPULL | GPIO_CFG_IN_INT_NONE,
};

/*
 * Array of callback function pointers
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP432P401R.h
 * NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *       reduce memory usage (if placed at end of gpioPinConfigs array).
 */
GPIO_CallbackFxn gpioCallbackFunctions[] = {NULL, NULL};

const GPIOMSP432_Config GPIOMSP432_config = {
    .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
    .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
    .numberOfPinConfigs = sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    .numberOfCallbacks =
        sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
    .intPriority = (~0)};

/*
 *  =============================== I2C ===============================
 */
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CMSP432.h>

I2CMSP432_Object i2cMSP432Objects[Board_I2CCOUNT];

const I2CMSP432_HWAttrsV1 i2cMSP432HWAttrs[Board_I2CCOUNT] = {
    {
        .baseAddr = EUSCI_B0_BASE,
        .intNum = INT_EUSCIB0,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .dataPin = I2CMSP432_P1_6_UCB0SDA,
        .clkPin = I2CMSP432_P1_7_UCB0SCL,
    },
    {
        .baseAddr = EUSCI_B1_BASE,
        .intNum = INT_EUSCIB1,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .dataPin = I2CMSP432_P6_4_UCB1SDA,
        .clkPin = I2CMSP432_P6_5_UCB1SCL,
    },
    {
        .baseAddr = EUSCI_B3_BASE,
        .intNum = INT_EUSCIB3,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .dataPin = I2CMSP432_P10_2_UCB3SDA,
        .clkPin = I2CMSP432_P10_3_UCB3SCL,
    },
    {
        .baseAddr = EUSCI_B2_BASE,
        .intNum = INT_EUSCIB2,
        .intPriority = (~0),
        .clockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
        .dataPin = I2CMSP432_P3_6_UCB2SDA,
        .clkPin = I2CMSP432_P3_7_UCB2SCL,
    },
};

const I2C_Config I2C_config[Board_I2CCOUNT] = {
    {.fxnTablePtr = &I2CMSP432_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_A],
     .hwAttrs = &i2cMSP432HWAttrs[I2C_BUS_A]},
    {.fxnTablePtr = &I2CMSP432_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_B],
     .hwAttrs = &i2cMSP432HWAttrs[I2C_BUS_B]},
    {.fxnTablePtr = &I2CMSP432_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_C],
     .hwAttrs = &i2cMSP432HWAttrs[I2C_BUS_C]},
    {.fxnTablePtr = &I2CMSP432_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_D],
     .hwAttrs = &i2cMSP432HWAttrs[I2C_BUS_D]},
};

const uint_least8_t I2C_count = Board_I2CCOUNT;

/*
 *  =============================== Power ===============================
 */
const PowerMSP432_ConfigV1 PowerMSP432_config = {
    .policyInitFxn = &PowerMSP432_initPolicy,
    .policyFxn = &PowerMSP432_sleepPolicy,
    .initialPerfLevel = 2,
    .enablePolicy = true,
    .enablePerf = true,
    .enableParking = true};

/*
 *  =============================== PWM ===============================
 */
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTimerMSP432.h>

PWMTimerMSP432_Object pwmTimerMSP432Objects[MSP_EXP432P401R_PWMCOUNT];

const PWMTimerMSP432_HWAttrsV2 pwmTimerMSP432HWAttrs[MSP_EXP432P401R_PWMCOUNT] =
    {{
         .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
         .pwmPin = PWMTimerMSP432_P2_1_TA1CCR1A  // use
                                                 // PWMTimerMSP432_P5_6_TA2CCR1A
                                                 // for pin 5.6 ta2.1
     },
     {.clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
      .pwmPin = PWMTimerMSP432_P2_5_TA1CCR2A},
     {.clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
      .pwmPin = PWMTimerMSP432_P2_6_TA1CCR3A}};

const PWM_Config PWM_config[MSP_EXP432P401R_PWMCOUNT] = {
    {.fxnTablePtr = &PWMTimerMSP432_fxnTable,
     .object = &pwmTimerMSP432Objects[MSP_EXP432P401R_PWM_TA1_1],
     .hwAttrs = &pwmTimerMSP432HWAttrs[MSP_EXP432P401R_PWM_TA1_1]},
    {.fxnTablePtr = &PWMTimerMSP432_fxnTable,
     .object = &pwmTimerMSP432Objects[MSP_EXP432P401R_PWM_TA1_2],
     .hwAttrs = &pwmTimerMSP432HWAttrs[MSP_EXP432P401R_PWM_TA1_2]},
    {.fxnTablePtr = &PWMTimerMSP432_fxnTable,
     .object = &pwmTimerMSP432Objects[MSP_EXP432P401R_PWM_TA1_3],
     .hwAttrs = &pwmTimerMSP432HWAttrs[MSP_EXP432P401R_PWM_TA1_3]}};

const uint_least8_t PWM_count = MSP_EXP432P401R_PWMCOUNT;

/*
 *  =============================== SPI ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIMSP432DMA.h>

/* SPI objects */
SPIMSP432DMA_Object spiMSP432DMAObjects[Board_SPICOUNT];

/* SPI configuration structure */
const SPIMSP432DMA_HWAttrsV1 spiMSP432DMAHWAttrs[Board_SPICOUNT] = {
    {.baseAddr = EUSCI_A0_BASE,
     .bitOrder = EUSCI_A_SPI_LSB_FIRST,
     .clockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK,
     .defaultTxBufValue = 0,
     .dmaIntNum = INT_DMA_INT1,
     .intPriority = (~0),
     .rxDMAChannelIndex = DMA_CH1_EUSCIA0RX,
     .txDMAChannelIndex = DMA_CH0_EUSCIA0TX,
     .clkPin = SPIMSP432DMA_P1_1_UCA0CLK,
     .simoPin = SPIMSP432DMA_P1_3_UCA0SIMO,
     .somiPin = SPIMSP432DMA_P1_2_UCA0SOMI,
     .pinMode = EUSCI_SPI_3PIN},
};

const SPI_Config SPI_config[Board_SPICOUNT] = {
    {.fxnTablePtr = &SPIMSP432DMA_fxnTable,
     .object = &spiMSP432DMAObjects[NVM],
     .hwAttrs = &spiMSP432DMAHWAttrs[NVM]},
};

const uint_least8_t SPI_count = Board_SPICOUNT;

/*
 *  =============================== Timer ===============================
 */
#include <ti/drivers/Timer.h>
#include <ti/drivers/timer/TimerMSP432.h>

TimerMSP432_Object timerMSP432Objects[MSP_EXP432P401R_TIMERCOUNT];

const TimerMSP432_HWAttrs timerMSP432HWAttrs[MSP_EXP432P401R_TIMERCOUNT] = {
    /* Timer32_0 */
    {.timerBaseAddress = TIMER32_0_BASE,
     .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
     .intNum = INT_T32_INT1,
     .intPriority = ~0},
    {.timerBaseAddress = TIMER32_1_BASE,
     .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,
     .intNum = INT_T32_INT2,
     .intPriority = ~0},
    /* Timer_A1 */
    {.timerBaseAddress = TIMER_A1_BASE,
     .clockSource = TIMER_A_CLOCKSOURCE_ACLK,
     .intNum = INT_TA1_0,
     .intPriority = ~0},
    /* Timer_A2 */
    {.timerBaseAddress = TIMER_A2_BASE,
     .clockSource = TIMER_A_CLOCKSOURCE_ACLK,
     .intNum = INT_TA2_0,
     .intPriority = ~0},
    /* Timer_A3 */
    {.timerBaseAddress = TIMER_A3_BASE,
     .clockSource = TIMER_A_CLOCKSOURCE_ACLK,
     .intNum = INT_TA3_0,
     .intPriority = ~0}};

const Timer_Config Timer_config[MSP_EXP432P401R_TIMERCOUNT] = {
    {.fxnTablePtr = &TimerMSP432_Timer32_fxnTable,
     .object = &timerMSP432Objects[MSP_EXP432P401R_TIMER_T32_0],
     .hwAttrs = &timerMSP432HWAttrs[MSP_EXP432P401R_TIMER_T32_0]},
    {.fxnTablePtr = &TimerMSP432_Timer32_fxnTable,
     .object = &timerMSP432Objects[MSP_EXP432P401R_TIMER_T32_1],
     .hwAttrs = &timerMSP432HWAttrs[MSP_EXP432P401R_TIMER_T32_1]},
    {.fxnTablePtr = &TimerMSP432_Timer_A_fxnTable,
     .object = &timerMSP432Objects[MSP_EXP432P401R_TIMER_TA_1],
     .hwAttrs = &timerMSP432HWAttrs[MSP_EXP432P401R_TIMER_TA_1]},
    {.fxnTablePtr = &TimerMSP432_Timer_A_fxnTable,
     .object = &timerMSP432Objects[MSP_EXP432P401R_TIMER_TA_2],
     .hwAttrs = &timerMSP432HWAttrs[MSP_EXP432P401R_TIMER_TA_2]},
    {.fxnTablePtr = &TimerMSP432_Timer_A_fxnTable,
     .object = &timerMSP432Objects[MSP_EXP432P401R_TIMER_TA_3],
     .hwAttrs = &timerMSP432HWAttrs[MSP_EXP432P401R_TIMER_TA_3]}};

const uint_least8_t Timer_count = MSP_EXP432P401R_TIMERCOUNT;

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>

UARTMSP432_Object uartMSP432Objects[Board_UARTCOUNT];
unsigned char uartMSP432RingBuffer[Board_UARTCOUNT][32];

/*
 * The baudrate dividers were determined by using the MSP432 baudrate
 * calculator
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const UARTMSP432_BaudrateConfig uartMSP432Baudrates[] = {
    /* {baudrate, input clock, prescalar, UCBRFx, UCBRSx, oversampling} */
    {.outputBaudrate = 115200,
     .inputClockFreq = 12000000,
     .prescalar = 6,
     .hwRegUCBRFx = 8,
     .hwRegUCBRSx = 32,
     .oversampling = 1},
    {115200, 6000000, 3, 4, 2, 1},
    {115200, 3000000, 1, 10, 0, 1},
    {9600, 12000000, 78, 2, 0, 1},
    {9600, 6000000, 39, 1, 0, 1},
    {9600, 3000000, 19, 8, 85, 1},
    {9600, 32768, 3, 0, 146, 0}};

const UARTMSP432_HWAttrsV1 uartMSP432HWAttrs[Board_UARTCOUNT] = {
    {
        .baseAddr = EUSCI_A2_BASE,
        .intNum = INT_EUSCIA2,
        .intPriority = (~0),
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries =
            sizeof(uartMSP432Baudrates) / sizeof(UARTMSP432_BaudrateConfig),
        .baudrateLUT = uartMSP432Baudrates,
        .ringBufPtr = uartMSP432RingBuffer[UART_MCU_MCU],
        .ringBufSize = sizeof(uartMSP432RingBuffer[UART_MCU_MCU]),
        .rxPin = UARTMSP432_P3_3_UCA2RXD,
        .txPin = UARTMSP432_P3_2_UCA2TXD,
    },
#ifdef SATELLITE_CONFIG
    {
        .baseAddr = EUSCI_A3_BASE,
        .intNum = INT_EUSCIA3,
        .intPriority = (~0),
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries =
            sizeof(uartMSP432Baudrates) / sizeof(UARTMSP432_BaudrateConfig),
        .baudrateLUT = uartMSP432Baudrates,
        .ringBufPtr = uartMSP432RingBuffer[UART_CDH_UMB],
        .ringBufSize = sizeof(uartMSP432RingBuffer[UART_CDH_UMB]),
        .rxPin = UARTMSP432_P9_6_UCA3RXD,
        .txPin = UARTMSP432_P9_7_UCA3TXD,
    },
#else
    {.baseAddr = EUSCI_A0_BASE,
     .intNum = INT_EUSCIA0,
     .intPriority = (~0),
     .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     .bitOrder = EUSCI_A_UART_LSB_FIRST,
     .numBaudrateEntries =
         sizeof(uartMSP432Baudrates) / sizeof(UARTMSP432_BaudrateConfig),
     .baudrateLUT = uartMSP432Baudrates,
     .ringBufPtr = uartMSP432RingBuffer[UART_CDH_UMB],
     .ringBufSize = sizeof(uartMSP432RingBuffer[UART_CDH_UMB]),
     .rxPin = UARTMSP432_P1_2_UCA0RXD,
     .txPin = UARTMSP432_P1_3_UCA0TXD},
#endif
    {
        .baseAddr = EUSCI_A1_BASE,
        .intNum = INT_EUSCIA1,
        .intPriority = (~0),
        .clockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        .bitOrder = EUSCI_A_UART_LSB_FIRST,
        .numBaudrateEntries =
            sizeof(uartMSP432Baudrates) / sizeof(UARTMSP432_BaudrateConfig),
        .baudrateLUT = uartMSP432Baudrates,
        .ringBufPtr = uartMSP432RingBuffer[UART_CMS_CDH],
        .ringBufSize = sizeof(uartMSP432RingBuffer[UART_CMS_CDH]),
        .rxPin = UARTMSP432_P2_2_UCA1RXD,
        .txPin = UARTMSP432_P2_3_UCA1TXD,
    },
};

const UART_Config UART_config[Board_UARTCOUNT] = {
    {.fxnTablePtr = &UARTMSP432_fxnTable,
     .object = &uartMSP432Objects[UART_MCU_MCU],
     .hwAttrs = &uartMSP432HWAttrs[UART_MCU_MCU]},
    {.fxnTablePtr = &UARTMSP432_fxnTable,
     .object = &uartMSP432Objects[UART_CDH_UMB],
     .hwAttrs = &uartMSP432HWAttrs[UART_CDH_UMB]},
    {.fxnTablePtr = &UARTMSP432_fxnTable,
     .object = &uartMSP432Objects[UART_CMS_CDH],
     .hwAttrs = &uartMSP432HWAttrs[UART_CMS_CDH]},
};

const uint_least8_t UART_count = Board_UARTCOUNT;

/*
 *  =============================== Watchdog ===============================
 */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP432.h>

WatchdogMSP432_Object watchdogMSP432Objects[MSP_EXP432P401R_WATCHDOGCOUNT];

const WatchdogMSP432_HWAttrs
    watchdogMSP432HWAttrs[MSP_EXP432P401R_WATCHDOGCOUNT] = {
        {.baseAddr = WDT_A_BASE,
         .intNum = INT_WDT_A,
         .intPriority = (~0),
         .clockSource = WDT_A_CLOCKSOURCE_SMCLK,
         .clockDivider = WDT_A_CLOCKDIVIDER_8192K}};

const Watchdog_Config Watchdog_config[MSP_EXP432P401R_WATCHDOGCOUNT] = {
    {.fxnTablePtr = &WatchdogMSP432_fxnTable,
     .object = &watchdogMSP432Objects[MSP_EXP432P401R_WATCHDOG],
     .hwAttrs = &watchdogMSP432HWAttrs[MSP_EXP432P401R_WATCHDOG]}};

const uint_least8_t Watchdog_count = MSP_EXP432P401R_WATCHDOGCOUNT;
