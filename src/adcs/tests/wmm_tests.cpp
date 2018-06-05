#include <external/wmm/worldMagneticModel.h>

#include <test_runners/unity.h>

void TestWorldMagneticModel() {
    r_vector b_vector = MagModel(2017.5, 0, -80, 240);
    TEST_ASSERT_FLOAT_WITHIN(0.1,b_vector.x, 5873.8);
    TEST_ASSERT_FLOAT_WITHIN(0.1,b_vector.y, 15781.4);
    TEST_ASSERT_FLOAT_WITHIN(0.1,b_vector.z, -52687.9);
}
