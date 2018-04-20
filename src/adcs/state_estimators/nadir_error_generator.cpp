#include <src/adcs/state_estimators/nadir_error_generator.h>

void ErrorQuaternionCalculatorMarkely(Matrix &ref_quaternion,
                                      Matrix &nadir_vector,
                                      Matrix error_quaternion) {
    ref_quaternion.QuaternionConjugate();
    double scale_ref_quaternion =
        Matrix::VectorNorm(ref_quaternion) * Matrix::VectorNorm(ref_quaternion);
    ref_quaternion.MultiplyScalar(
        ref_quaternion,
        scale_ref_quaternion);  // inverts ref quaternion quaternion
    error_quaternion.QuaternionProductCross(nadir_vector, ref_quaternion);
    double scale_error = 1.0 / Matrix::VectorNorm(error_quaternion);
    error_quaternion.MultiplyScalar(error_quaternion, scale_error);
    return;
}
void ErrorQuaternionCalculatorEarthSensor(Matrix &ref_quaternion,
                                          Matrix &nadir_quaternion,
                                          Matrix &error_quaternion) {
    double ref_vector[3][1];
    double nadir_vector[3][1];
    double crossData[3][1];
    for (uint8_t i = 0; i < 3; i++) {
        ref_vector[i][0] = ref_quaternion.Get(i + 1, 0);
        nadir_vector[i][0] = nadir_quaternion.Get(i + 1, 0);
    }
    Matrix a(ref_vector);
    Matrix b(nadir_vector);
    Matrix cross(crossData);

    double dot_product = Matrix::DotProduct(a, b);
    double magA = Matrix::VectorNorm(a);
    double magB = Matrix::VectorNorm(b);
    double dot = dot_product / magA / magB;
    double theta = acos(dot);

    cross.CrossProduct(b, a);

    error_quaternion.Set(0, 0, cos(theta / 2));
    double x = cross.Get(0, 0) * sin(theta / 2);
    double y = cross.Get(1, 0) * sin(theta / 2);
    double z = cross.Get(2, 0) * sin(theta / 2);

    error_quaternion.Set(1, 0, x);
    error_quaternion.Set(2, 0, y);
    error_quaternion.Set(3, 0, z);

    return;
}
