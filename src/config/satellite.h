#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

constexpr bool kEnterDeepSleepOnStartup = false;
static constexpr bool kInstantDeploymentWaits = true;
static constexpr bool kLogI2c = false;
static constexpr bool kLithiumTransmitOnlyWhenGroundCommanded = false;
static constexpr bool kLogToUart = true;
static constexpr bool kLogToSd = true;
static constexpr bool kFormatSdOnStartup = false;
static constexpr uint8_t kNominalLithiumPowerLevel =
    140;  // Scaled between 0-255

static constexpr bool kLimpModeEnabled = false;
static constexpr bool kUseFecDefault = true;
static constexpr bool kCheckHmacDefault = true;
static constexpr bool kCheckSequenceDefault = false;

// Task timing
static constexpr uint32_t kNominalBeaconPeriodMs = 10000;
static constexpr uint32_t kOrientationControlLoopPeriodMicros = 1000000;
static constexpr uint32_t kHealthCheckPeriodMs = 1100;
static constexpr uint64_t kAntennaBurnCheckIntervalMs = 60 * 1000;
static constexpr uint32_t kTimeUpdatePeriodMs = 1000;
static constexpr uint16_t kIChargeHoldTimeMs = 5000;
static constexpr uint16_t kIChargeHoldPollMs = 200;

// Wait times during boot
static constexpr uint32_t kBeaconWaitMs = 5 * 60 * 1000;
static constexpr uint32_t kEjectionWaitMs = 30 * 60 * 1000;

// Time taken for the solar panels to unlatch after the end of a beacon
static constexpr uint32_t kSolarPowerRecoveryTimeMs = 300;

// Initialise BMS with 91mA battery-charge current
static constexpr uint16_t kInitialIChargeIndex = 1;

// Orbital parameters
static uint64_t kHalfOrbitPeriodMs = 46.5 * 60 * 1000;

// DO NOT TURN ON UNTIL LAUNCH
static bool kDeployAntenna = false;
static uint64_t kInitialAntennaBurnIntervalMs = kHalfOrbitPeriodMs;
static uint64_t kBackupAntennaBurnIntervalMs = 3 * kHalfOrbitPeriodMs;
static float kAntennaBurnIntervalMultiplier = 2.5;

// Will disable orientation control if true
// MUST BE 'false' ON LAUNCH
static bool kRunMagnetorquersAtConstantPower = true;
// float value in range [-1, 1]
static float kMagnetorquerPowerFractionX = 0.2;
static float kMagnetorquerPowerFractionY = 0.2;
static float kMagnetorquerPowerFractionZ = 0.2;

// The strength of the magnetorquers
const double kMaxMagnetorquerDipole[3] = {0.2, 0.2, 0.05};

// Scale the actuation strength for power budgeting
const double kOrientationControlPowerLevel = 0.2;

// Mapping from magnetometer frames to satellite body frame
// TODO (rskew) verify these for the final build
// Mappings for the Helmholtz rig FS board
const double kImuAToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
extern double kImuAToBodyFrameTransform_dummy_data[3][3];
const Matrix kImuAToBodyFrameTransform(kImuAToBodyFrameTransform_const_data,
                                       kImuAToBodyFrameTransform_dummy_data);
const double kImuBToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
extern double kImuBToBodyFrameTransform_dummy_data[3][3];
const Matrix kImuBToBodyFrameTransform(kImuBToBodyFrameTransform_const_data,
                                       kImuBToBodyFrameTransform_dummy_data);

// Mapping from body frame to magnetorquer 'frame'
// TODO (rskew) update for final build
// Mapping for the Helmholtz rig
const double kBodyToMagnetorquerFrameTransform_const_data[3][3] = {
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
static bool kUsePreFlightMagnetometerCalibrationScaleFactors = true;

// TODO (rskew) populate these values with calibration parameters
// from pre-flight calibration of the flight model
const double kPreFlightMagnetometerCalibrationBiasesImuBusA[3][1] = {
    {0}, {0}, {0}};
const double kPreFlightMagnetometerCalibrationBiasesImuBusB[3][1] = {
    {0}, {0}, {0}};
const double kPreFlightMagnetometerCalibrationScaleFactorsImuBusA[3][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
const double kPreFlightMagnetometerCalibrationScaleFactorsImuBusB[3][3] = {
    {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

#endif  //  SRC_CONFIG_SATELLITE_H_
