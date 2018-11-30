#ifndef SRC_CONFIG_ORIENTATION_CONTROL_PARAMETERS_H_
#define SRC_CONFIG_ORIENTATION_CONTROL_PARAMETERS_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

const uint16_t kBDotEstimatorTimeConstantMillis = 4000;

const double kBDotControllerGains[3] = {30000.0, 30000.0, 30000.0};

// Will disable orientation control if true, must be FALSE until launch
constexpr bool kRunMagnetorquersAtConstantPower = false;

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

#endif  //  SRC_CONFIG_ORIENTATION_CONTROL_PARAMETERS_H_
