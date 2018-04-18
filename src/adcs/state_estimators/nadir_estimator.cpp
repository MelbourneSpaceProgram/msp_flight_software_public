#include <src/adcs/state_estimators/nadir_estimator.h>

void ErrorQuaternionCalculatorMarkely(Matrix &reference, Matrix &nadir_vector, Matrix error_quaternion)
{
    reference.QuaternionConjugate();
    double scale_reference  = VectorNorm(reference) * VectorNorm(reference);
    reference.MultiplyScalar(reference, scalar_reference); // inverts reference quarternion
    error_quarternion.QuaternionProductCross(nadir_vector, reference);
    double scale_error = 1.0/ VectorNorm(error_quaternion);
    error_quaternion.MultiplyScalar(error_quaternion, scale_error);
    return;

}
void ErrorQuaternionCalculatorEarthSensor(Matrix &reference, Matrix &nadir_vector, Matrix error_quaternion)
{
    return;
}



