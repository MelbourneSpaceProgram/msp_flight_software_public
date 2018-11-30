#include <CppUTest/TestHarness.h>
#include <src/adcs/kalman_filter.h>
#include <src/util/matrix.h>

TEST_GROUP(KalmanFilter){};

TEST(KalmanFilter, PredictAndUpdate) {
    const double r1_data[3][1] = {{0.3554}, {0.0734}, {-0.9318}};
    NewConstStackMatrixMacro(r1, 3, 1, r1_data);
    const double r2_data[3][1] = {{0}, {0}, {1}};
    NewConstStackMatrixMacro(r2, 3, 1, r2_data);
    const double q0_data[4][1] = {{0}, {0}, {0}, {1}};
    NewConstStackMatrixMacro(q0, 4, 1, q0_data);
    const double P0_data[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    NewConstStackMatrixMacro(P0, 3, 3, P0_data);
    const double Q0_data[3][3] = {{0.1, 0, 0}, {0, 0.1, 0}, {0, 0, 0.1}};
    NewConstStackMatrixMacro(Q0, 3, 3, Q0_data);
    const double R0_data[6][6] = {{0.1, 0, 0, 0, 0, 0}, {0, 0.1, 0, 0, 0, 0},
                                  {0, 0, 0.1, 0, 0, 0}, {0, 0, 0, 0.1, 0, 0},
                                  {0, 0, 0, 0, 0.1, 0}, {0, 0, 0, 0, 0, 0.1}};
    NewConstStackMatrixMacro(R0, 6, 6, R0_data);

    double y1_data[3][1] = {{-0.366}, {-0.5753}, {-0.7315}};
    Matrix y1(y1_data);
    double y2_data[3][1] = {{0.5844}, {0.2708}, {0.7650}};
    Matrix y2(y2_data);
    double omega_data[3][1] = {{0}, {0.4363}, {-0.0873}};
    Matrix omega(omega_data);
    double q_prior_estimate_expected_data[4][1] = {
        {0}, {0.0109}, {-0.0022}, {1}};
    Matrix q_prior_estimate_expected(q_prior_estimate_expected_data);
    NewStackMatrixMacro(P_prior_expected, 3, 3);
    P_prior_expected.Identity();
    P_prior_expected.MultiplyScalar(P_prior_expected, 0.005);
    double q_posterior_estimate_expected_data[4][1] = {
        {0.01947041}, {-0.01670298}, {0.00216513}, {0.99966856}};
    Matrix q_posterior_estimate_expected(q_posterior_estimate_expected_data);
    double P_posterior_expected_data[3][3] = {
        {0.00457628, 0.00000612, -0.00008373},
        {0.00000612, 0.00454669, -0.00001576},
        {-0.00008373, -0.00001576, 0.00496511}};
    Matrix P_posterior_expected(P_posterior_expected_data);

    KalmanFilter kf(50, Q0, R0, P0, q0);
    kf.UpdateRef1(r1);
    kf.UpdateRef2(r2);

    kf.Predict(omega);

    // Test q_estimate prediction
    CHECK(q_prior_estimate_expected.IsEqual(kf.q_estimate));

    // Test P prediction
    CHECK(P_prior_expected.IsEqual(kf.P));

    kf.Update(y1, y2);

    // Test q_estimate update
    CHECK(q_posterior_estimate_expected.IsEqual(kf.q_estimate));

    // Test P update
    CHECK(P_posterior_expected.IsEqual(kf.P));
}
