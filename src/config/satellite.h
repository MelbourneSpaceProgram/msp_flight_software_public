#ifndef SRC_CONFIG_SATELLITE_H_
#define SRC_CONFIG_SATELLITE_H_

#include <src/util/data_types.h>
#include <src/util/matrix.h>

static const bool kLithiumTransmitOnlyWhenGroundCommanded = false;
static const uint32_t kNominalBeaconPeriodMs = 10000;  // In milliseconds
static const uint8_t kNominalLithiumPowerLevel = 150;  // Scaled between 0-255

// Will disable orientation control if true
static bool kRunMagnetorquersAtConstantPower = false;
// float value in range [-1, 1]
static float kMagnetorquerPowerFractionX = 1.0;
static float kMagnetorquerPowerFractionY = 1.0;
static float kMagnetorquerPowerFractionZ = 1.0;

// Extended kalman Filter tuning constant matrices
// TODO (rskew) update tuning
double ekf_process_noise_tuning_data[3][3] = {{0.1, 0  , 0  },
                                              {0  , 0.1, 0  },
                                              {0  , 0  , 0.1}};
Matrix ekf_process_noise_tuning(ekf_process_noise_tuning_data);
double ekf_sensor_noise_tuning_data[6][6] = {{0.1, 0  , 0  , 0  , 0  , 0  },
                                             {0  , 0.1, 0  , 0  , 0  , 0  },
                                             {0  , 0  , 0.1, 0  , 0  , 0  },
                                             {0  , 0  , 0  , 0.1, 0  , 0  },
                                             {0  , 0  , 0  , 0  , 0.1, 0  },
                                             {0  , 0  , 0  , 0  , 0  , 0.1}};
Matrix ekf_sensor_noise_tuning(ekf_sensor_noise_tuning_data);
double ekf_initial_covariance_data[3][3] = {{0.1, 0  , 0  },
                                            {0  , 0.1, 0  },
                                            {0  , 0  , 0.1}};
Matrix ekf_initial_covariance(ekf_initial_covariance_data);
double ekf_initial_quaternion_estimate_data[4][1] = {{0},{0},{0},{1}};
Matrix ekf_initial_quaternion_estimate(ekf_initial_quaternion_estimate_data);

#endif  //  SRC_CONFIG_SATELLITE_H_
