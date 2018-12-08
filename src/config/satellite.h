#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

//////////////////// HARDWARE ////////////////////

constexpr bool kEpsBoardAvailable = true;
constexpr bool kCdhBoardAvailable = true;
constexpr bool kFsBoardAvailable = true;
constexpr bool kTcomBoardAvailable = true;
constexpr bool kUtilitiesBoardAvailable = true;

constexpr bool kXPosAvailable = true;
constexpr bool kXNegAvailable = true;
constexpr bool kYPosAvailable = true;
constexpr bool kYNegAvailable = true;
constexpr bool kZPosAvailable = true;
constexpr bool kZNegAvailable = true;

constexpr bool kI2cAvailable = true;
constexpr bool kSdCardAvailable = false;
constexpr bool kBmsAvailable = true;
constexpr bool kAntennaAvailable = true;

// Eeprom is actually available, but resource conflicts with SD card as they
// share the same bus
constexpr bool kEepromAvailable = false;

//////////////////// BOOT ////////////////////

constexpr bool kEnterDeepSleepOnStartup = false;
constexpr bool kEnterLimpModeOnStartup = false;
constexpr bool kDeployAntenna = true;  // DO NOT TURN ON UNTIL AFTER LAUNCH

//////////////////// WAITS ////////////////////

constexpr bool kInstantDeploymentWaits = false;
constexpr bool kFormatSdOnStartup = false;
constexpr uint32_t kBeaconWaitMs = 5 * 60 * 1000 + 5;
constexpr uint32_t kEjectionWaitMs = 30 * 60 * 1000;
constexpr uint64_t kTimeSourceDeployMs = 0;
constexpr uint32_t kMuxResetWaitMs = 5;

//////////////////// TELECOMMS ////////////////////

constexpr bool kLithiumTransmitOnlyWhenGroundCommanded = false;
constexpr uint8_t kNominalLithiumPowerLevel = 140;  // Scaled between 0-255

constexpr bool kUplinkFecEnabled = true;
constexpr bool kDownlinkFecEnabled = true;
constexpr bool kCheckHmacDefault = true;
constexpr bool kCheckSequenceDefault = false;

//////////////////// LOGGING & TESTING ////////////////////

constexpr bool kLogI2c = false;
constexpr bool kLogToUart = true;
constexpr bool kLogToSd = false;
constexpr bool kHilAvailable = false;
constexpr bool kDitlMode = false;
constexpr bool kVerboseLogging = false;
constexpr bool kRebootOnTopLevelException = true;
constexpr bool kFlushUartOnHardFault = false;

//////////////////// TASK CONSTANTS ////////////////////

constexpr uint32_t kNominalBeaconPeriodMs = 10000;
constexpr uint32_t kOrientationControlLoopPeriodMicros = 1000000;
constexpr uint32_t kHealthCheckPeriodMs = 1100;
constexpr uint64_t kAntennaBurnCheckIntervalMs = 60 * 1000;
constexpr uint32_t kTimeUpdatePeriodMs = 1000;
constexpr uint16_t kIChargeHoldTimeMs = 5000;
constexpr uint16_t kIChargeHoldPollMs = 200;

//////////////////// SATELLITE POWER ////////////////////

// Time taken for the solar panels to unlatch after the end of a beacon
constexpr uint32_t kSolarPowerRecoveryTimeMs = 300;
// Initialise BMS with 91mA battery-charge current
constexpr uint16_t kInitialIChargeIndex = 1;

//////////////////// ANTENNA BURN ////////////////////

constexpr uint64_t kHalfOrbitPeriodMs = 46.5 * 60 * 1000;
constexpr uint64_t kInitialAntennaBurnIntervalMs = kHalfOrbitPeriodMs;
constexpr uint64_t kBackupAntennaBurnIntervalMs = 3 * kHalfOrbitPeriodMs;
constexpr float kAntennaBurnIntervalMultiplier = 2.5;

#endif  //  SRC_CONFIG_SATELLITE_H_
