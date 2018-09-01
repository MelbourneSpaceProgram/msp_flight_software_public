#include <src/board/MSP432E.h>
#include <ti/devices/msp432e4/driverlib/driverlib.h>
#include <ti/drivers/Power.h>

/*
 *  =============================== GPIO ===============================
 */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP432E4.h>
/*
 * Array of Pin configurations
 * NOTE: The order of the pin configurations must coincide with what was
 *       defined in MSP_EXP432P401R.h
 * NOTE: Pins not used for interrupts should be placed at the end of the
 *       array.  Callback entries can be omitted from callbacks array to
 *       reduce memory usage.
 */
GPIO_PinConfig gpioPinConfigs[] = {
    GPIOMSP432E4_PL2 | GPIO_CFG_OUT_OD_PU | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PL3 | GPIO_CFG_INPUT |
        GPIO_CFG_IN_INT_NONE,  // TODO(akremor): Configure interrupt for RTC if
                               // used
    GPIOMSP432E4_PQ0 | GPIO_CFG_INPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PQ1 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PQ2 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PQ3 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PQ4 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PP2 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PC6 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PC7 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PD2 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PD3 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PK0 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PK1 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PK2 | GPIO_CFG_OUTPUT |
        GPIO_CFG_IN_INT_NONE,  // TODO(akremor): Needs configuration
    GPIOMSP432E4_PK3 | GPIO_CFG_OUTPUT,
    GPIOMSP432E4_PH0 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PE0 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PE1 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
    GPIOMSP432E4_PE2 | GPIO_CFG_OUTPUT | GPIO_CFG_IN_INT_NONE,
};
// Array of callback function pointers
// NOTE: The order of the pin configurations must coincide with what was
//       defined in MSP_EXP432E401Y.h
// NOTE: Pins not used for interrupts can be omitted from callbacks array to
// reduce memory usage (if placed at end of gpioPinConfigs array).
const GPIO_CallbackFxn gpioCallbackFunctions[] = {};

// The device-specific GPIO_config structure
const GPIOMSP432E4_Config GPIOMSP432E4_config = {
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
#include <ti/drivers/i2c/I2CMSP432E4.h>

I2CMSP432E4_Object i2cMSP432Objects[Board_I2CCOUNT];

const I2CMSP432E4_HWAttrs i2cMSP432E4HWAttrs[] = {
    {.baseAddr = I2C2_BASE,
     .intNum = INT_I2C2,
     .intPriority = (~0),
     .sclPin = I2CMSP432E4_PL1_I2C2SCL,
     .sdaPin = I2CMSP432E4_PL0_I2C2SDA},
    {.baseAddr = I2C0_BASE,
     .intNum = INT_I2C0,
     .intPriority = (~0),
     .sclPin = I2CMSP432E4_PB2_I2C0SCL,
     .sdaPin = I2CMSP432E4_PB3_I2C0SDA},
    {.baseAddr = I2C7_BASE,
     .intNum = INT_I2C7,
     .intPriority = (~0),
     .sclPin = I2CMSP432E4_PD0_I2C7SCL,
     .sdaPin = I2CMSP432E4_PD1_I2C7SDA},
    {.baseAddr = I2C3_BASE,
     .intNum = INT_I2C3,
     .intPriority = (~0),
     .sclPin = I2CMSP432E4_PK4_I2C3SCL,
     .sdaPin = I2CMSP432E4_PK5_I2C3SDA},
};

const I2C_Config I2C_config[Board_I2CCOUNT] = {
    {.fxnTablePtr = &I2CMSP432E4_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_A],
     .hwAttrs = &i2cMSP432E4HWAttrs[I2C_BUS_A]},
    {.fxnTablePtr = &I2CMSP432E4_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_B],
     .hwAttrs = &i2cMSP432E4HWAttrs[I2C_BUS_B]},
    {.fxnTablePtr = &I2CMSP432E4_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_C],
     .hwAttrs = &i2cMSP432E4HWAttrs[I2C_BUS_C]},
    {.fxnTablePtr = &I2CMSP432E4_fxnTable,
     .object = &i2cMSP432Objects[I2C_BUS_D],
     .hwAttrs = &i2cMSP432E4HWAttrs[I2C_BUS_D]}};

const uint_least8_t I2C_count = Board_I2CCOUNT;

/*
 *  =============================== PWM ===============================
 */
#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMMSP432E4.h>

PWMMSP432E4_Object pwmMSP432Objects[Board_PWMCOUNT];

const PWMMSP432E4_HWAttrs PWMMSP432E4HWAttrs[] = {
    {.pwmBaseAddr = PWM0_BASE,
     .pwmOutput = PWM_OUT_0,
     .pwmGenOpts = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN,
     .pinConfig = PWMMSP432E4_PF0_M0PWM0},
    {.pwmBaseAddr = PWM0_BASE,
     .pwmOutput = PWM_OUT_1,
     .pwmGenOpts = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN,
     .pinConfig = PWMMSP432E4_PF1_M0PWM1},
    {.pwmBaseAddr = PWM0_BASE,
     .pwmOutput = PWM_OUT_2,
     .pwmGenOpts = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN,
     .pinConfig = PWMMSP432E4_PF2_M0PWM2},
};

const PWM_Config PWM_config[Board_PWMCOUNT] = {
    {.fxnTablePtr = &PWMMSP432E4_fxnTable,
     .object = &pwmMSP432Objects[Mag_STR_CDH_FS_Out1],
     .hwAttrs = &PWMMSP432E4HWAttrs[Mag_STR_CDH_FS_Out1]},
    {.fxnTablePtr = &PWMMSP432E4_fxnTable,
     .object = &pwmMSP432Objects[Mag_STR_CDH_FS_Out2],
     .hwAttrs = &PWMMSP432E4HWAttrs[Mag_STR_CDH_FS_Out2]},
    {.fxnTablePtr = &PWMMSP432E4_fxnTable,
     .object = &pwmMSP432Objects[Mag_STR_CDH_FS_Out3],
     .hwAttrs = &PWMMSP432E4HWAttrs[Mag_STR_CDH_FS_Out3]}};

const uint_least8_t PWM_count = Board_PWMCOUNT;

/*
 *  =============================== SDFatFS ===============================
 */
#include <ti/drivers/SDFatFS.h>

/*
 * Note: The SDFatFS driver provides interface functions to enable FatFs
 * but relies on the SD driver to communicate with SD cards.  Opening a
 * SDFatFs driver instance will internally try to open a SD driver instance
 * reusing the same index number (opening SDFatFs driver at index 0 will try to
 * open SD driver at index 0).  This requires that all SDFatFs driver instances
 * have an accompanying SD driver instance defined with the same index.  It is
 * acceptable to have more SD driver instances than SDFatFs driver instances
 * but the opposite is not supported & the SDFatFs will fail to open.
 */
SDFatFS_Object sdfatfsObjects[MSP_EXP432P401R_SDFatFSCOUNT];

const SDFatFS_Config SDFatFS_config[MSP_EXP432P401R_SDFatFSCOUNT] = {
    {.object = &sdfatfsObjects[MSP_EXP432P401R_SDFatFS0]}};

const uint_least8_t SDFatFS_count = MSP_EXP432P401R_SDFatFSCOUNT;

/*
 *  =============================== SD ===============================
 */
#include <ti/drivers/SD.h>
#include <ti/drivers/sd/SDSPI.h>

SDSPI_Object sdspiObjects[MSP_EXP432P401R_SDCOUNT];

const SDSPI_HWAttrs sdspiHWAttrs[MSP_EXP432P401R_SDCOUNT] = {
    {.spiIndex = 0, .spiCsGpioIndex = SYS_nCS1}};

const SD_Config SD_config[MSP_EXP432P401R_SDCOUNT] = {
    {.fxnTablePtr = &SDSPI_fxnTable,
     .object = &sdspiObjects[MSP_EXP432P401R_SDSPI0],
     .hwAttrs = &sdspiHWAttrs[MSP_EXP432P401R_SDSPI0]},
};

const uint_least8_t SD_count = MSP_EXP432P401R_SDCOUNT;

/*
 *  =============================== DMA ===============================
 */
#include <ti/drivers/dma/UDMAMSP432E4.h>

#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(dmaControlTable, 256)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 256
#elif defined(__GNUC__)
__attribute__((aligned(256)))
#endif
static tDMAControlTable dmaControlTable[64];

/*
 *  ======== dmaErrorHwi ========
 *  This is the handler for the uDMA error interrupt.
 */
static void dmaErrorHwi(uintptr_t arg) {
    int status = uDMAErrorStatusGet();
    uDMAErrorStatusClear();

    // Suppress unused variable warning
    (void)status;

    while (1)
        ;
}

UDMAMSP432E4_Object udmaMSP432Object;

const UDMAMSP432E4_HWAttrs udmaMSP432HWAttrs = {
    .controlBaseAddr = (void *)dmaControlTable,
    .dmaErrorFxn = (UDMAMSP432E4_ErrorFxn)dmaErrorHwi,
    .intNum = INT_UDMAERR,
    .intPriority = (~0)};

const UDMAMSP432E4_Config UDMAMSP432E4_config = {.object = &udmaMSP432Object,
                                                 .hwAttrs = &udmaMSP432HWAttrs};

void initGeneral(void) {
    /* Grant the DMA access to all FLASH memory */
    FLASH_CTRL->PP |= FLASH_PP_DFA;

    /* Region start address - match FLASH start address */
    FLASH_CTRL->DMAST = 0x00000000;

    /*
     * Access to FLASH is granted to the DMA in 2KB regions.  The value
     * assigned to DMASZ is the amount of 2KB regions to which the DMA will
     * have access.  The value can be determined via the following:
     *     2 * (num_regions + 1) KB
     *
     * To grant full access to entire 1MB of FLASH:
     *     2 * (511 + 1) KB = 1024 KB (1 MB)
     */
    FLASH_CTRL->DMASZ = 511;
}

/*
 *  =============================== SPI ===============================
 */
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPIMSP432E4DMA.h>

SPIMSP432E4DMA_Object spiMSP432DMAObjects[Board_SPICOUNT];

uint16_t spiMSP432E4DMAscratchBuf;
const SPIMSP432E4DMA_HWAttrs spiMSP432E4DMAobjects[] = {
    {.baseAddr = SSI1_BASE,
     .intNum = INT_SSI1,
     .intPriority = (~0),
     .scratchBufPtr = &spiMSP432E4DMAscratchBuf,
     .defaultTxBufValue = 0,
     .rxDmaChannel = UDMA_CH24_SSI1RX,
     .txDmaChannel = UDMA_CH25_SSI1TX,
     .minDmaTransferSize = 10,
     .clkPinMask = SPIMSP432E4_PB5_SSI1CLK,
     .fssPinMask = SPIMSP432E4_PIN_NO_CONFIG,
     .xdat0PinMask = SPIMSP432E4_PE4_SSI1XDAT0,
     .xdat1PinMask = SPIMSP432E4_PE5_SSI1XDAT1}};

const SPI_Config SPI_config[Board_SPICOUNT] = {
    {.fxnTablePtr = &SPIMSP432E4DMA_fxnTable,
     .object = &spiMSP432DMAObjects[NVM],
     .hwAttrs = &spiMSP432E4DMAobjects[NVM]}};

const uint_least8_t SPI_count = Board_SPICOUNT;

/*
 *  =============================== Timer ===============================
 */
// TODO(akremor): Bring back in
/*
#include <ti/drivers/Timer.h>
#include <ti/drivers/timer/TimerMSP432E4.h>
const TimerMSP432E4_HWAttrs timerMSP432E4HWAttrs[] = {
    {.baseAddress = TIMER2_BASE,
     .subTimer = TimerMSP432E4_timer32,
     .intNum = INT_TIMER02A,
     .intPriority = ~0},
    {.baseAddress = TIMER1_BASE,
     .subTimer = TimerMSP432E4_timer16A,
     .intNum = INT_TIMER1A,
     .intPriority = ~0},
    {.baseAddress = TIMER1_BASE,
     .subTimer = TimerMSP432E4_timer16B,
     .intNum = INT_TIMER1B,
     .intPriority = ~0}};

const uint_least8_t Timer_count = MSP_EXP432P401R_TIMERCOUNT;
*/

/*
 *  =============================== UART ===============================
 */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432E4.h>

UARTMSP432E4_Object uartMSP432Objects[Board_UARTCOUNT];
unsigned char uartMSP432E4RingBuffer[Board_UARTCOUNT][32];

const UARTMSP432E4_HWAttrs uartMSP432E4HWAttrs[] = {
    {.baseAddr = UART0_BASE,
     .intNum = INT_UART0,
     .intPriority = (~0),
     .flowControl = UARTMSP432E4_FLOWCTRL_NONE,
     .ringBufPtr = uartMSP432E4RingBuffer[0],
     .ringBufSize = sizeof(uartMSP432E4RingBuffer[0]),
     .rxPin = UARTMSP432E4_PA0_U0RX,
     .txPin = UARTMSP432E4_PA1_U0TX,
     .ctsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .rtsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .errorFxn = NULL},
    {.baseAddr = UART3_BASE,
     .intNum = INT_UART3,
     .intPriority = (~0),
     .flowControl = UARTMSP432E4_FLOWCTRL_NONE,
     .ringBufPtr = uartMSP432E4RingBuffer[1],
     .ringBufSize = sizeof(uartMSP432E4RingBuffer[1]),
     .rxPin = UARTMSP432E4_PA4_U3RX,
     .txPin = UARTMSP432E4_PA5_U3TX,
     .ctsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .rtsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .errorFxn = NULL},
    {.baseAddr = UART2_BASE,
     .intNum = INT_UART2,
     .intPriority = (~0),
     .flowControl = UARTMSP432E4_FLOWCTRL_NONE,
     .ringBufPtr = uartMSP432E4RingBuffer[2],
     .ringBufSize = sizeof(uartMSP432E4RingBuffer[2]),
     .rxPin = UARTMSP432E4_PA6_U2RX,
     .txPin = UARTMSP432E4_PA7_U2TX,
     .ctsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .rtsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .errorFxn = NULL},
    {.baseAddr = UART1_BASE,
     .intNum = INT_UART1,
     .intPriority = (~0),
     .flowControl = UARTMSP432E4_FLOWCTRL_NONE,
     .ringBufPtr = uartMSP432E4RingBuffer[3],
     .ringBufSize = sizeof(uartMSP432E4RingBuffer[3]),
     .rxPin = UARTMSP432E4_PB0_U1RX,
     .txPin = UARTMSP432E4_PB1_U1TX,
     .ctsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .rtsPin = UARTMSP432E4_PIN_UNASSIGNED,
     .errorFxn = NULL}};

const UART_Config UART_config[Board_UARTCOUNT] = {
    {.fxnTablePtr = &UARTMSP432E4_fxnTable,
     .object = &uartMSP432Objects[0],
     .hwAttrs = &uartMSP432E4HWAttrs[0]},
    {.fxnTablePtr = &UARTMSP432E4_fxnTable,
     .object = &uartMSP432Objects[1],
     .hwAttrs = &uartMSP432E4HWAttrs[1]},
    {.fxnTablePtr = &UARTMSP432E4_fxnTable,
     .object = &uartMSP432Objects[2],
     .hwAttrs = &uartMSP432E4HWAttrs[2]},
    {.fxnTablePtr = &UARTMSP432E4_fxnTable,
     .object = &uartMSP432Objects[3],
     .hwAttrs = &uartMSP432E4HWAttrs[3]},
};

const uint_least8_t UART_count = Board_UARTCOUNT;

/*
 *  =============================== Watchdog ===============================
 */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogMSP432E4.h>

WatchdogMSP432E4_Object watchdogMSP432Objects[Board_WATCHDOGCOUNT];

const WatchdogMSP432E4_HWAttrs watchdogMSP432E4HWAttrs[] = {
    {
        .baseAddr = WATCHDOG0_BASE,
        .intNum = INT_WATCHDOG,
        .intPriority = ~0,
        .reloadValue = 10 * 120000000  // 10 seconds at 120MHz system clock
    },
};

const Watchdog_Config Watchdog_config[Board_WATCHDOGCOUNT] = {
    {.fxnTablePtr = &WatchdogMSP432E4_fxnTable,
     .object = &watchdogMSP432Objects[SYS_WATCHDOG0],
     .hwAttrs = &watchdogMSP432E4HWAttrs[SYS_WATCHDOG0]}};

const uint_least8_t Watchdog_count = Board_WATCHDOGCOUNT;

void EnterLowPowerMode() {
    SysCtlPeripheralClockGating(true);
    SysCtlDeepSleepPowerSet(SYSCTL_SRAM_LOW_POWER | SYSCTL_FLASH_LOW_POWER);
    SysCtlDeepSleepClockConfigSet(
        (SYSCTL_DSLP_OSC_INT30 | SYSCTL_DSLP_PIOSC_PD | SYSCTL_DSLP_MOSC_DPD),
        1);
    SysCtlLDODeepSleepSet(SYSCTL_LDO_0_90V);
    SysCtlDeepSleep();
}
