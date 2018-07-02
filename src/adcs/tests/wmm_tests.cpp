#include <CppUTest/TestHarness.h>
#include <external/wmm/worldMagneticModel.h>

TEST_GROUP(WorldMagneticModel){};

TEST(WorldMagneticModel, TestWmm) {
    r_vector b_vector = MagModel(2017.5, 0, -80, 240);
    DOUBLES_EQUAL(5873.8, b_vector.x, 0.1);
    DOUBLES_EQUAL(15781.4, b_vector.y, 0.1);
    DOUBLES_EQUAL(-52687.9, b_vector.z, 0.1);
}
