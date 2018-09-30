#include <CppUTest/TestHarness.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/util/matrix.h>

TEST_GROUP(BDotEstimator){};

TEST(BDotEstimator, TestBDotEstimator) {
    // test on known sequence
    BDotEstimator b_dot_estimator(1, 1000);

    double input_total_data[3][9] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9},
        {654, 213, 73, 0.3, 1, 0.003, 0.6, -50, -500},
        {9, 8, 7, 6, 5, 4, 3, 2, 1}};
    Matrix input_total(input_total_data);

    double output_expected_total_data[3][9] = {
        {0.499750083312489, 1.498750749645925, 2.496752914646903,
         3.493757576317671, 4.489765731662972, 5.484778376691046,
         6.478796506414620, 7.471821114851906, 8.463853195027598},
        {326.8365544863678, 432.9566490412597, 142.1691103938455,
         35.7301696237515, -0.2875486852402, -0.4356870550252, -0.6351516190661,
         -25.6235202007914, -275.7728011943132},
        {4.497750749812401, 8.491255913688439, 7.483268735362874,
         6.476289040390002, 5.470315821790043, 4.465348073589694,
         3.461384790821122, 2.458424969520964, 1.456467606729316}};
    Matrix output_expected_total(output_expected_total_data);

    double output_data[3][1];
    Matrix output(output_data);
    double input_data[3][1];
    Matrix input(input_data);
    double output_expected_data[3][1];
    Matrix output_expected(output_expected_data);

    for (uint8_t i = 0; i < 9; i++) {
        input.CopySlice(0, 2, i, i, input_total);
        output_expected.CopySlice(0, 2, i, i, output_expected_total);
        b_dot_estimator.Estimate(input, output);

        CHECK(output_expected.IsEqual(output));
    }

    // test failure on wrong sized args
    double m32_data[3][2];
    Matrix m32(m32_data);

    CHECK_THROWS(etl::exception, b_dot_estimator.Estimate(m32, output));
    CHECK_THROWS(etl::exception, b_dot_estimator.Estimate(input, m32));
}
