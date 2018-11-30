#include <src/util/data_types.h>
#include <src/util/matrix.h>

constexpr uint16_t kMissionYear = 2019;

// TODO (rskew) add the units for this to the name
constexpr double kCriticallyFastSpin = 100;
constexpr double kSpinSlowEnoughForEarthPointing = 0.01;

constexpr uint16_t kBDotEstimatorTimeConstantMillis = 4000;
// TODO (rskew) experimentally derive a value
constexpr uint16_t kAngularVelocitySmootherTimeConstantMillis = 4000;

constexpr double kBDotControllerGains[3] = {30000.0, 30000.0, 30000.0};

constexpr double kNadirControllerKp = 0.02;
constexpr double kNadirControllerKd = 7.0;

constexpr double kKFProcessNoise_data[3][3] = {
    {0.1, 0, 0}, {0, 0.1, 0}, {0, 0, 0.1}};
NewConstStackMatrixMacro(kKFProcessNoise, 3, 3, kKFProcessNoise_data);

constexpr double kKFSensorNoise_data[6][6] = {
    {0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0}, {0, 0, 0.1, 0, 0, 0},
    {0, 0, 0, 0.1, 0, 0}, {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}};
NewConstStackMatrixMacro(kKFSensorNoise, 6, 6, kKFSensorNoise_data);

constexpr double kKFInitialCovariance_data[3][3] = {
    {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
NewConstStackMatrixMacro(kKFInitialCovariance, 3, 3, kKFInitialCovariance_data);

constexpr double kKFInitialEstimate_data[4][1] = {{1}, {0}, {0}, {0}};
NewConstStackMatrixMacro(kKFInitialEstimate, 4, 1, kKFInitialEstimate_data);
