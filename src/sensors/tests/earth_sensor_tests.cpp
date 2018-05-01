#include <math.h>
#include <src/sensors/earth_sensor.h>
#include <src/sensors/infrared_sensor.h>
#include <test_runners/unity.h>

void TestInfraredToAngle(void) {
    double side_normal_data[3][1] = {{1}, {0}, {0}};
    Matrix side_normal(side_normal_data);
    InfraredSensor test_sensor(side_normal);

    double expected_angle_1 = 0.024022641263855;
    double expected_angle_2 = 1.318405182716569;
    double expected_angle_3 = 2.973281695942479;

    test_sensor.SetInfraredReading(1.0);
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, expected_angle_1,
                              test_sensor.GetAngleToNadir());
    test_sensor.SetInfraredReading(0.5);
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, expected_angle_2,
                              test_sensor.GetAngleToNadir());
    test_sensor.SetInfraredReading(0.0);
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, expected_angle_3,
                              test_sensor.GetAngleToNadir());
}

void TestCalculateNadirVector(void) {
    double nadir_expected_data_1[3][1] = {
        {-0.404816393349314}, {-0.859347789667761}, {0.452032066153492}};
    double nadir_vector_data[3][1];
    Matrix nadir_vector(nadir_vector_data);
    Matrix nadir_expected_1(nadir_expected_data_1);
    EarthSensor earth_sensor;

    earth_sensor.SetPosXSensorReading(0.133315183411290);
    earth_sensor.SetPosYSensorReading(0.004293997614282);
    earth_sensor.SetPosZSensorReading(0.636557561183074);
    earth_sensor.SetNegXSensorReading(0.595124837677689);
    earth_sensor.SetNegYSensorReading(0.807433740690897);
    earth_sensor.SetNegZSensorReading(0.097213969998926);

    earth_sensor.CalculateNadirVector();
    nadir_vector = earth_sensor.GetNadirVector();
    TEST_ASSERT_DOUBLE_WITHIN(1e-4, nadir_expected_1.Get(0, 0),
                              nadir_vector.Get(0, 0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-4, nadir_expected_1.Get(1, 0),
                              nadir_vector.Get(1, 0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-4, nadir_expected_1.Get(2, 0),
                              nadir_vector.Get(2, 0));
}
