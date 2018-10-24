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
static constexpr uint32_t kNominalBeaconPeriodMs = 10000;  // In milliseconds
static constexpr uint8_t kNominalLithiumPowerLevel = 150;  // Scaled between 0-255

// DO NOT TURN ON UNTIL LAUNCH
static bool kDeployAntenna = false;
// Will disable orientation control if true
static bool kRunMagnetorquersAtConstantPower = false;
// float value in range [-1, 1]
static float kMagnetorquerPowerFractionX = 1.0;
static float kMagnetorquerPowerFractionY = 1.0;
static float kMagnetorquerPowerFractionZ = 1.0;

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
