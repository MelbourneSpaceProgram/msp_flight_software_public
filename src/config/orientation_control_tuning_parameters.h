#include <src/util/data_types.h>
#include <src/util/matrix.h>

const uint16_t kBDotEstimatorTimeConstantMillis = 10;

const double kBDotControllerGains[3] = {30000.0, 30000.0, 30000.0};

//////////////////// MAGNETORQUERS ////////////////////

// Will disable orientation control if true, must be FALSE until launch
constexpr bool kRunMagnetorquersAtConstantPower = true;

// float value in range [-1, 1]
constexpr float kMagnetorquerPowerFractionX = 1.0;
constexpr float kMagnetorquerPowerFractionY = 0.0;
constexpr float kMagnetorquerPowerFractionZ = 0.0;

// The strength of the magnetorquers
constexpr double kMaxMagnetorquerDipole[3] = {0.2, 0.2, 0.2};

// Scale the actuation strength for power budgeting
constexpr double kOrientationControlPowerLevel = 1.0;

// Mapping from magnetometer frames to satellite body frame
// TODO (rskew) verify these for the final build
// Mappings for the Helmholtz rig FS board
constexpr double kImuAToBodyFrameTransform_const_data[3][3] = {
    {0, 1, 0}, {1, 0, 0}, {0, 0, -1}};
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
    //{1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
    {-1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
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

