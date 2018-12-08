#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>
#include <src/database/flash_memory/flash_storables/system_configuration.h>

constexpr bool kFlushUartOnHardFault = false;

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
