#include <src/util/data_types.h>
#include <src/util/matrix.h>

const uint16_t kBDotEstimatorTimeConstantMillis = 30;

const double kBDotControllerGains[3] = {5000.0, 5000.0, 5000.0};

//////////////////// MAGNETORQUERS ////////////////////

// Will disable orientation control if true, must be FALSE on launch
constexpr bool kRunMagnetorquersAtConstantPower = false;

// float value in range [-1, 1]
constexpr float kMagnetorquerPowerFractionX = 0.0;
constexpr float kMagnetorquerPowerFractionY = 0.0;
constexpr float kMagnetorquerPowerFractionZ = 0.0;

// The strength of the magnetorquers
constexpr double kMaxMagnetorquerDipole[3] = {0.2, 0.2, 0.2};

// Scale the actuation strength for power budgeting
constexpr double kOrientationControlPowerLevel = 0.2;

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
    {1, 0, 0}, {0, -1, 0}, {0, 0, -1}};
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
    {16.298290525}, {-27.343861702}, {17.907423506}};
constexpr double kPreFlightMagnetometerCalibrationBiasesImuBusB[3][1] = {
    {5.035408134}, {-32.052124763}, {44.668596412}};
constexpr double kPreFlightMagnetometerCalibrationScaleFactorsImuBusA[3][3] = {
    {1.384082, -0.129752, 0.064525},
    {-0.129752, 1.264288, 0.206924},
    {0.064525, 0.206924, 1.038411}};
constexpr double kPreFlightMagnetometerCalibrationScaleFactorsImuBusB[3][3] = {
    {1.210644, 0.073275, 0.063186},
    {0.073275, 1.223178, -0.017730},
    {0.063186, -0.017730, 1.094803}};
