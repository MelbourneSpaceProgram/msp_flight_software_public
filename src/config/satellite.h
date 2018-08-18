#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>

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

#endif  //  SRC_CONFIG_SATELLITE_H_
