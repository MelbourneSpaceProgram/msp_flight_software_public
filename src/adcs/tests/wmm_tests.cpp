#include <external/wmm/worldMagneticModel.h>

#include <test_runners/unity.h>
#include <test_runners/wmm_tests.h>

bool similar(double var1, double var2) {
    double err = fabs((var1 - var2) / var2 * 100);

    if (err < 0.1) {
        return true;
    } else {
        return false;
    }
}

void TestWorldMagneticModel() {
    r_vector b_vector = MagModel(2017.5, 0, -80, 240);
    TEST_ASSERT(similar(b_vector.x, 5873.8));
    TEST_ASSERT(similar(b_vector.y, 15781.4));
    TEST_ASSERT(similar(b_vector.z, -52687.9));
}
