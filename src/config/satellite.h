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
constexpr bool kSdCardAvailable = true;
constexpr bool kBmsAvailable = true;
constexpr bool kAntennaAvailable = false;

// Eeprom is actually available, but resource conflicts with SD card as they
// share the same bus
constexpr bool kEepromAvailable = false;

//////////////////// BOOT ////////////////////

constexpr bool kEnterDeepSleepOnStartup = false;
constexpr bool kEnterLimpModeOnStartup = false;
constexpr bool kDeployAntenna = false;  // DO NOT TURN ON UNTIL AFTER LAUNCH

//////////////////// WAITS ////////////////////

constexpr bool kInstantDeploymentWaits = true;
constexpr bool kFormatSdOnStartup = false;
constexpr uint32_t kBeaconWaitMs = 5 * 60 * 1000;
constexpr uint32_t kEjectionWaitMs = 30 * 60 * 1000;
constexpr uint64_t kTimeSourceDeployMs = 0;

//////////////////// TELECOMMS ////////////////////

constexpr bool kLithiumTransmitOnlyWhenGroundCommanded = false;
constexpr uint8_t kNominalLithiumPowerLevel = 140;  // Scaled between 0-255

constexpr bool kUseFecDefault = true;
constexpr bool kCheckHmacDefault = true;
constexpr bool kCheckSequenceDefault = false;

//////////////////// LOGGING & TESTING ////////////////////

constexpr bool kLogI2c = false;
constexpr bool kLogToUart = true;
constexpr bool kLogToSd = true;
constexpr bool kHilAvailable = false;
constexpr bool kDitlMode = false;
constexpr bool kVerboseLogging = false;
constexpr bool kRebootOnTopLevelException = true;

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

//////////////////// MAGNETORQUERS ////////////////////

// Will disable orientation control if true, must be FALSE until launch
constexpr bool kRunMagnetorquersAtConstantPower = true;

// float value in range [-1, 1]
constexpr float kMagnetorquerPowerFractionX = 0.2;
constexpr float kMagnetorquerPowerFractionY = 0.2;
constexpr float kMagnetorquerPowerFractionZ = 0.2;

// The strength of the magnetorquers
constexpr double kMaxMagnetorquerDipole[3] = {0.2, 0.2, 0.2};

// Scale the actuation strength for power budgeting
constexpr double kOrientationControlPowerLevel = 0.2;

// Mapping from magnetometer frames to satellite body frame
// TODO (rskew) verify these for the final build
// Mappings for the Helmholtz rig FS board
constexpr double kImuAToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
extern double kImuAToBodyFrameTransform_dummy_data[3][3];

const Matrix kImuAToBodyFrameTransform(kImuAToBodyFrameTransform_const_data,
                                       kImuAToBodyFrameTransform_dummy_data);
constexpr double kImuBToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
extern double kImuBToBodyFrameTransform_dummy_data[3][3];
const Matrix kImuBToBodyFrameTransform(kImuBToBodyFrameTransform_const_data,
                                       kImuBToBodyFrameTransform_dummy_data);

// Mapping from body frame to magnetorquer 'frame'
// TODO (rskew) update for final build
// Mapping for the Helmholtz rig
constexpr double kBodyToMagnetorquerFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
extern double kBodyToMagnetorquerFrameTransform_dummy_data[3][3];
const Matrix kBodyToMagnetorquerFrameTransform(
    kBodyToMagnetorquerFrameTransform_const_data,
    kBodyToMagnetorquerFrameTransform_dummy_data);

// Won't override the hard-coded initial scale factors that are set with
// pre-flight calibration values if true.
//
// Because the on-orbit calibration data is taken orbiting the Earth, the
// readings will have different magnitudes for different locations,
// and the location of the spacecraft is not known during the comissioning
// period. This will corrupt the generated scale factors, therefore the
// pre-flight calibration scale factors are to be used.
constexpr bool kUsePreFlightMagnetometerCalibrationScaleFactors = true;

// TODO (rskew) populate these values with calibration parameters
// from pre-flight calibration of the flight model
constexpr double kPreFlightMagnetometerCalibrationBiasesImuBusA[3][1] = {
    {0}, {0}, {0}};
constexpr double kPreFlightMagnetometerCalibrationBiasesImuBusB[3][1] = {
    {0}, {0}, {0}};
constexpr double kPreFlightMagnetometerCalibrationScaleFactorsImuBusA[3][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
constexpr double kPreFlightMagnetometerCalibrationScaleFactorsImuBusB[3][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

#endif  //  SRC_CONFIG_SATELLITE_H_
