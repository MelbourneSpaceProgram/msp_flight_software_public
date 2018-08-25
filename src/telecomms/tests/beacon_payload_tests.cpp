#include <CppUTest/TestHarness.h>
#include <src/telecomms/msp_payloads/beacon_payload.h>

TEST_GROUP(BeaconPayload){};

TEST(BeaconPayload, TestCurrentData) {
    BeaconPayload test_payload = new BeaconPayload();

    float test_current = BeaconPayload::kCurrentUpperBound/2;
    float test_ub = BeaconPayload::kCurrentUpperBound;
    float test_lb = -BeaconPayload::kCurrentUpperBound;
    float test_greater_ub = BeaconPayload::kCurrentUpperBound*2;
    float test_below_lb = -BeaconPayload::kCurrentUpperBound*2;

    test_payload.SetCommsRegulator1OutputCurrent(test_current);

    CHECK_EQUAL(test_payload.CommsRegulator1OutputCurrent(),
                test_current);

    test_payload.SetCommsRegulator1OutputCurrent(test_ub);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputCurrent(),
                BeaconPayload::kCurrentUpperBound);

    test_payload.SetCommsRegulator1OutputCurrent(test_lb);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputCurrent(),
                -BeaconPayload::kCurrentUpperBound);

    test_payload.SetCommsRegulator1OutputCurrent(test_greater_lb);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputCurrent(),
                BeaconPayload::kCurrentUpperBound);

    test_payload.SetCommsRegulator1OutputCurrent(test_below_lb);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputCurrent(),
                -BeaconPayload::kCurrentUpperBound);

    free(test_payload);
}

TEST(BeaconPayload, TestVoltageData) {
    BeaconPayload test_payload = new BeaconPayload();

    float test_voltage = BeaconPayload::kVoltageUpperBound/2;
    float test_ub = BeaconPayload::kVoltageUpperBound;
    float test_lb = -BeaconPayload::kVoltageUpperBound/2;
    float test_greater_ub = BeaconPayload::kVoltageUpperBound*2;
    float test_below_lb = -BeaconPayload::kVoltageUpperBound*2;

    test_payload.SetCommsRegulator1OutputVoltage(test_voltage);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputVoltage(),
                test_voltage);

    test_payload.SetCommsRegulator1OutputVoltage(test_ub);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputVoltage(),
                BeaconPayload::kVoltageUpperBound);

    test_payload.SetCommsRegulator1OutputVoltage(test_lb);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputVoltage(),
                -BeaconPayload::kVoltageUpperBound);

    test_payload.SetCommsRegulator1OutputVoltage(test_greater_lb);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputVoltage(),
                BeaconPayload::kVoltageUpperBound);

    test_payload.SetCommsRegulator1OutputVoltage(test_below_lb);

    CHECK_EQUAL(test_payload.GetCommsRegulator1OutputVoltage(),
                -BeaconPayload::kVoltageUpperBound);

    free(test_payload);
}

TEST(BeaconPayload, TestTempData) {
  BeaconPayload test_payload = new BeaconPayload();

  byte test_temp = BeaconPayload::kTempUpperBound/2;
  byte test_ub = BeaconPayload::kTempUpperBound;
  byte test_lb = -BeaconPayload::kTempUpperBound;
  byte test_greater_ub = BeaconPayload::kTempUpperBound*2;
  byte test_below_lb = -BeaconPayload::kTempUpperBound*2;

  test_payload.SetCommsTemp2(test_temp);

  CHECK_EQUAL(test_payload.GetCommsTemp2(),
              test_temp);

  test_payload.SetCommsTemp2(test_ub);

  CHECK_EQUAL(test_payload.GetCommsTemp2(),
              BeaconPayload::kTempUpperBound);

  test_payload.SetCommsTemp2(test_lb);

  CHECK_EQUAL(test_payload.GetCommsTemp2(),
              -BeaconPayload::kTempUpperBound);

  test_payload.SetCommsTemp2(test_greater_lb);

  CHECK_EQUAL(test_payload.GetCommsTemp2(),
              BeaconPayload::kTempUpperBound);

  test_payload.SetCommsTemp2(test_below_lb);

  CHECK_EQUAL(test_payload.GetCommsTemp2(),
              -BeaconPayload::kTempUpperBound);

  free(test_payload);
}
