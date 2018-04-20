#ifndef SRC_ADCS_STATE_ESTIMATORS_NADIR_ERROR_GENERATOR_H_
#define SRC_ADCS_STATE_ESTIMATORS_NADIR_ERROR_GENERATOR_H_

#include <math.h>
#include <src/util/matrix.h>

void ErrorQuaternionCalculatorEarthSensor(Matrix &ref_quaternion,
                                          Matrix &nadir_quaternion,
                                          Matrix &error_quaternion);
void ErrorQuaternionCalculatorMarkely(Matrix &ref_quaternion,
                                      Matrix &nadir_vector,
                                      Matrix error_quaternion);

#endif /* SRC_ADCS_STATE_ESTIMATORS_NADIR_ERROR_GENERATOR_H_ */
