#include <CppUTest/TestHarness.h>
#include <src/adcs/state_estimators/b_dot_estimator.h>
#include <src/messages/MagnetometerReading.pb.h>
#include <src/util/matrix.h>

TEST_GROUP(BDotEstimator){};

TEST(BDotEstimator, TestBDotEstimator) {
    // test on known sequence
    BDotEstimator b_dot_estimator(1, 1000);
    MagnetometerReading magnetometer_reading1;
    magnetometer_reading1.x = 1;
    magnetometer_reading1.y = 654;
    magnetometer_reading1.z = 9;
    MagnetometerReading magnetometer_reading2;
    magnetometer_reading2.x = 2;
    magnetometer_reading2.y = 213;
    magnetometer_reading2.z = 8;
    MagnetometerReading magnetometer_reading3;
    magnetometer_reading3.x = 3;
    magnetometer_reading3.y = 73;
    magnetometer_reading3.z = 7;
    MagnetometerReading magnetometer_reading4;
    magnetometer_reading4.x = 4;
    magnetometer_reading4.y = 0.3;
    magnetometer_reading4.z = 6;
    MagnetometerReading magnetometer_reading5;
    magnetometer_reading5.x = 5;
    magnetometer_reading5.y = 1;
    magnetometer_reading5.z = 5;
    MagnetometerReading magnetometer_reading6;
    magnetometer_reading6.x = 6;
    magnetometer_reading6.y = 0.003;
    magnetometer_reading6.z = 4;
    MagnetometerReading magnetometer_reading7;
    magnetometer_reading7.x = 7;
    magnetometer_reading7.y = 0.6;
    magnetometer_reading7.z = 3;
    MagnetometerReading magnetometer_reading8;
    magnetometer_reading8.x = 8;
    magnetometer_reading8.y = -50;
    magnetometer_reading8.z = 2;
    MagnetometerReading magnetometer_reading9;
    magnetometer_reading9.x = 9;
    magnetometer_reading9.y = -500;
    magnetometer_reading9.z = 1;
    MagnetometerReading magnetometer_readings[9] = {
        magnetometer_reading1, magnetometer_reading2, magnetometer_reading3,
        magnetometer_reading4, magnetometer_reading5, magnetometer_reading6,
        magnetometer_reading7, magnetometer_reading8, magnetometer_reading9};

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

    NewStackMatrixMacro(output, 3, 1);
    NewStackMatrixMacro(input, 3, 1);
    NewStackMatrixMacro(output_expected, 3, 1);

    for (uint8_t i = 0; i < 9; i++) {
        output_expected.CopySlice(0, 2, i, i, output_expected_total);
        b_dot_estimator.Estimate(magnetometer_readings[i], output);

        CHECK(output_expected.IsEqual(output));
    }

    // test failure on wrong sized args
    NewStackMatrixMacro(m32, 3, 2);

    CHECK_THROWS(MspException,
                 b_dot_estimator.Estimate(magnetometer_reading1, m32));
}
