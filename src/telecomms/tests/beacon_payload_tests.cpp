#include <CppUTest/TestHarness.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>
#include <stdint.h>

TEST_GROUP(BeaconPayload){};

TEST(BeaconPayload, TestConstrain) {
    float test_current = BeaconPayload::kCurrentUpperBound / 2;
    float test_ub = BeaconPayload::kCurrentUpperBound;
    float test_lb = -BeaconPayload::kCurrentUpperBound;
    float test_greater_ub = BeaconPayload::kCurrentUpperBound * 2;
    float test_below_lb = -BeaconPayload::kCurrentUpperBound * 2;

    CHECK_EQUAL(test_current,
                BeaconPayload::ConstrainToRange(
                    test_current, BeaconPayload::kCurrentUpperBound));
    CHECK_EQUAL(BeaconPayload::kCurrentUpperBound,
                BeaconPayload::ConstrainToRange(
                    test_ub, BeaconPayload::kCurrentUpperBound));
    CHECK_EQUAL(-BeaconPayload::kCurrentUpperBound,
                BeaconPayload::ConstrainToRange(
                    test_lb, BeaconPayload::kCurrentUpperBound));
    CHECK_EQUAL(BeaconPayload::kCurrentUpperBound,
                BeaconPayload::ConstrainToRange(
                    test_greater_ub, BeaconPayload::kCurrentUpperBound));
    CHECK_EQUAL(-BeaconPayload::kCurrentUpperBound,
                BeaconPayload::ConstrainToRange(
                    test_below_lb, BeaconPayload::kCurrentUpperBound));
}

TEST(BeaconPayload, TestScaling) {
    CHECK_EQUAL(1000, BeaconPayload::ScaleArbitraryInt16(500, 16384));
    CHECK_EQUAL(32767, BeaconPayload::ScaleArbitraryInt16(1213, 1213));
    CHECK_EQUAL(-32767, BeaconPayload::ScaleArbitraryInt16(-1213, 1213));
    CHECK_EQUAL(0, BeaconPayload::ScaleArbitraryInt16(0, 1213))
    CHECK_EQUAL(993, BeaconPayload::ScaleArbitraryInt16(231, 7623));
    CHECK_EQUAL(-993, BeaconPayload::ScaleArbitraryInt16(-231, 7623));
    CHECK_EQUAL(INT16_MIN, BeaconPayload::ScaleArbitraryInt16(-9999, 12345));
}
