#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

//////////////////// HARDWARE ////////////////////
static constexpr bool kEpsBoardAvailable = true;
static constexpr bool kCdhBoardAvailable = true;
static constexpr bool kFsBoardAvailable = true;
static constexpr bool kTcomBoardAvailable = true;
static constexpr bool kUtilitiesBoardAvailable = true;

static constexpr bool kXPosAvailable = true;
static constexpr bool kXNegAvailable = true;
static constexpr bool kYPosAvailable = true;
static constexpr bool kYNegAvailable = true;
static constexpr bool kZPosAvailable = true;
static constexpr bool kZNegAvailable = true;

static constexpr bool kI2cAvailable = true;
static constexpr bool kSdCardAvailable = true;
static constexpr bool kBmsAvailable = true;
static constexpr bool kAntennaAvailable = false;

// Eeprom is actually available, but resource conflicts with SD card as they
// share the same bus
static constexpr bool kEepromAvailable = false;

//////////////////// BOOT ////////////////////

static constexpr bool kEnterDeepSleepOnStartup = false;
static constexpr bool kLimpModeEnabled = false;
static constexpr bool kDeployAntenna =
    false;  // DO NOT TURN ON UNTIL AFTER LAUNCH

//////////////////// WAITS ////////////////////

static constexpr bool kInstantDeploymentWaits = true;
static constexpr bool kFormatSdOnStartup = false;
static constexpr uint32_t kBeaconWaitMs = 5 * 60 * 1000;
static constexpr uint32_t kEjectionWaitMs = 30 * 60 * 1000;
static constexpr uint64_t kTimeSourceDeployMs = 0;

//////////////////// TELECOMMS ////////////////////

static constexpr bool kLithiumTransmitOnlyWhenGroundCommanded = false;
static constexpr uint8_t kNominalLithiumPowerLevel =
    140;  // Scaled between 0-255

static constexpr bool kUseFecDefault = true;
static constexpr bool kCheckHmacDefault = true;
static constexpr bool kCheckSequenceDefault = false;

//////////////////// LOGGING & TESTING ////////////////////

static constexpr bool kLogI2c = false;
static constexpr bool kLogToUart = true;
static constexpr bool kLogToSd = true;
static constexpr bool kHilAvailable = false;
static constexpr bool kDitlMode = false;
static constexpr bool kVerboseLogging = false;

//////////////////// TASK CONSTANTS ////////////////////

static constexpr uint32_t kNominalBeaconPeriodMs = 10000;
static constexpr uint32_t kOrientationControlLoopPeriodMicros = 1000000;
static constexpr uint32_t kHealthCheckPeriodMs = 1100;
static constexpr uint64_t kAntennaBurnCheckIntervalMs = 60 * 1000;
static constexpr uint32_t kTimeUpdatePeriodMs = 1000;
static constexpr uint16_t kIChargeHoldTimeMs = 5000;
static constexpr uint16_t kIChargeHoldPollMs = 200;

//////////////////// SATELLITE POWER ////////////////////

// Time taken for the solar panels to unlatch after the end of a beacon
static constexpr uint32_t kSolarPowerRecoveryTimeMs = 300;
// Initialise BMS with 91mA battery-charge current
static constexpr uint16_t kInitialIChargeIndex = 1;

//////////////////// ANTENNA BURN ////////////////////

static constexpr uint64_t kHalfOrbitPeriodMs = 46.5 * 60 * 1000;
static constexpr uint64_t kInitialAntennaBurnIntervalMs = kHalfOrbitPeriodMs;
static constexpr uint64_t kBackupAntennaBurnIntervalMs = 3 * kHalfOrbitPeriodMs;
static constexpr float kAntennaBurnIntervalMultiplier = 2.5;

//////////////////// MAGNETORQUERS ////////////////////

// Will disable orientation control if true, must be FALSE until launch
static constexpr bool kRunMagnetorquersAtConstantPower = true;

// float value in range [-1, 1]
static constexpr float kMagnetorquerPowerFractionX = 0.2;
static constexpr float kMagnetorquerPowerFractionY = 0.2;
static constexpr float kMagnetorquerPowerFractionZ = 0.2;

// The strength of the magnetorquers
static constexpr double kMaxMagnetorquerDipole[3] = {0.2, 0.2, 0.2};

// Scale the actuation strength for power budgeting
static constexpr double kOrientationControlPowerLevel = 0.2;

// Mapping from magnetometer frames to satellite body frame
// TODO (rskew) verify these for the final build
// Mappings for the Helmholtz rig FS board
static constexpr double kImuAToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
extern double kImuAToBodyFrameTransform_dummy_data[3][3];

static const Matrix kImuAToBodyFrameTransform(
    kImuAToBodyFrameTransform_const_data, kImuAToBodyFrameTransform_dummy_data);
static constexpr double kImuBToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
extern double kImuBToBodyFrameTransform_dummy_data[3][3];
static const Matrix kImuBToBodyFrameTransform(
    kImuBToBodyFrameTransform_const_data, kImuBToBodyFrameTransform_dummy_data);

// Mapping from body frame to magnetorquer 'frame'
// TODO (rskew) update for final build
// Mapping for the Helmholtz rig
static constexpr double kBodyToMagnetorquerFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
extern double kBodyToMagnetorquerFrameTransform_dummy_data[3][3];
static const Matrix kBodyToMagnetorquerFrameTransform(
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
static constexpr bool kUsePreFlightMagnetometerCalibrationScaleFactors = true;

// TODO (rskew) populate these values with calibration parameters
// from pre-flight calibration of the flight model
static constexpr double kPreFlightMagnetometerCalibrationBiasesImuBusA[3][1] = {
    {0}, {0}, {0}};
static constexpr double kPreFlightMagnetometerCalibrationBiasesImuBusB[3][1] = {
    {0}, {0}, {0}};
static constexpr double
    kPreFlightMagnetometerCalibrationScaleFactorsImuBusA[3][3] = {
        {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
static constexpr double
    kPreFlightMagnetometerCalibrationScaleFactorsImuBusB[3][3] = {
        {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

#endif  //  SRC_CONFIG_SATELLITE_H_
