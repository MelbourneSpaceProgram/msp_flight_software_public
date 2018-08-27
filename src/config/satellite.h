#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

static const bool kLithiumTransmitOnlyWhenGroundCommanded = false;
static const uint32_t kNominalBeaconPeriodMs = 10000;  // In milliseconds
static const uint8_t kNominalLithiumPowerLevel = 150;  // Scaled between 0-255

// DO NOT TURN ON UNTIL LAUNCH
static bool kDeployAntenna = false;
// Will disable orientation control if true
static bool kRunMagnetorquersAtConstantPower = true;
// float value in range [-1, 1]
static float kMagnetorquerPowerFractionX = 1.0;
static float kMagnetorquerPowerFractionY = 1.0;
static float kMagnetorquerPowerFractionZ = 1.0;

// Mapping from magnetometer frames to satellite body frame
// TODO (rskew) verify these for the final build
// Mappings for the Helmholtz rig FS board
const double kImuAToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
double kImuAToBodyFrameTransform_dummy_data[3][3];
const Matrix kImuAToBodyFrameTransform(kImuAToBodyFrameTransform_const_data,
                                       kImuAToBodyFrameTransform_dummy_data);
const double kImuBToBodyFrameTransform_const_data[3][3] = {
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
double kImuBToBodyFrameTransform_dummy_data[3][3];
const Matrix kImuBToBodyFrameTransform(kImuBToBodyFrameTransform_const_data,
                                       kImuBToBodyFrameTransform_dummy_data);

// Mapping from body frame to magnetorquer 'frame'
// TODO (rskew) update for final build
// Mapping for the Helmholtz rig
const double kBodyToMagnetorquerFrameTransform_const_data[3][3] = {
    {0, 0, 0}, {0, 1, 0}, {-1, 0, 0}};
double kBodyToMagnetorquerFrameTransform_dummy_data[3][3];
const Matrix kBodyToMagnetorquerFrameTransform(
    kBodyToMagnetorquerFrameTransform_const_data,
    kBodyToMagnetorquerFrameTransform_dummy_data);

#endif  //  SRC_CONFIG_SATELLITE_H_
