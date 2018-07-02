#ifndef SRC_BOARD_MSP_EXP432P401R_H_
#define SRC_BOARD_MSP_EXP432P401R_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum MSP_EXP432P401R_SDFatFSName {
    MSP_EXP432P401R_SDFatFS0 = 0,

    MSP_EXP432P401R_SDFatFSCOUNT
} MSP_EXP432P401R_SDFatFSName;

typedef enum MSP_EXP432P401R_SDName {
    MSP_EXP432P401R_SDSPI0 = 0,

    MSP_EXP432P401R_SDCOUNT
} MSP_EXP432P401R_SDName;

// TODO(akremor): Pins to boot MCU
// TODO(akremor): Power alert pins
typedef enum Board_GPIOName {
    I2C_MUX_nRST = 0,
    RTC_INT = 1,
    MCU_IDENTIFIER = 2,
    SYS_nCS1 = 3,
    SYS_nCS2 = 4,
    SYS_nCS3 = 5,
    SYS_LED = 6,
    SYS_nCS4 = 7,
    UNUSED_1 = 8,
    UNUSED_2 = 9,
    UNUSED_3 = 10,
    UNUSED_4 = 11,
    ANT_OVERRIDE_1 = 12,
    ANT_OVERRIDE_2 = 13,
    COMMS_INT = 14,
    COMMS_RST = 15,
    UNUSED_5 = 16,
    FS_X_DIR = 17,
    FS_Y_DIR = 18,
    FS_Z_DIR = 19,
    Board_GPIOCOUNT
} Board_GPIOName;

typedef enum Board_I2CName {
    I2C_BUS_A = 0,
    I2C_BUS_B = 1,
    I2C_BUS_C = 2,
    I2C_BUS_D = 3,
    Board_I2CCOUNT
} Board_I2CName;

typedef enum Board_PWMName {
    Mag_STR_CDH_FS_Out1 = 0,
    Mag_STR_CDH_FS_Out2 = 1,
    Mag_STR_CDH_FS_Out3 = 2,

    Board_PWMCOUNT
} Board_PWMName;

typedef enum Board_SPIName { NVM = 0, Board_SPICOUNT } Board_SPIName;

typedef enum MSP_EXP432P401R_TimerName {
    MSP_EXP432P401R_TIMER_T32_0 = 0,
    MSP_EXP432P401R_TIMER_T32_1,
    MSP_EXP432P401R_TIMER_TA_1,
    MSP_EXP432P401R_TIMER_TA_2,
    MSP_EXP432P401R_TIMER_TA_3,

    MSP_EXP432P401R_TIMERCOUNT
} MSP_EXP432P401R_TimerName;

typedef enum Board_UARTName {
    BOOT = 0,
    UMBILICAL_SIM = 1,
    TELECOMS = 2,
    UMBILICAL_CONSOLE = 3,
    Board_UARTCOUNT
} Board_UARTName;

typedef enum Board_WatchdogName {
    SYS_WATCHDOG0 = 0,
    Board_WATCHDOGCOUNT
} Board_WatchdogName;

#ifdef __cplusplus
}
#endif

#endif  // SRC_BOARD_MSP_EXP432P401R_H_
