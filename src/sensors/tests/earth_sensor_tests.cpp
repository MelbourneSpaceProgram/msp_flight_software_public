#include <CppUTest/TestHarness.h>
#include <math.h>
#include <src/sensors/earth_sensor.h>
#include <src/sensors/infrared_sensor.h>

TEST_GROUP(EarthSensor){};

TEST(EarthSensor, TestInfraredToAngle) {
    double side_normal_data[3][1] = {{1}, {0}, {0}};
    Matrix side_normal(side_normal_data);
    InfraredSensor test_sensor(side_normal);
    // Data obtained from MATLAB script code of the running algorithm
    double expected_angle_1 = 0.024022641263855;
    double expected_angle_2 = 1.318405182716569;
    double expected_angle_3 = 2.973281695942479;

    test_sensor.SetInfraredReading(1.0);
    DOUBLES_EQUAL(expected_angle_1, test_sensor.GetAngleToNadir(), 0.001);
    test_sensor.SetInfraredReading(0.5);
    DOUBLES_EQUAL(expected_angle_2, test_sensor.GetAngleToNadir(), 0.001);
    test_sensor.SetInfraredReading(0.0);
    DOUBLES_EQUAL(expected_angle_3, test_sensor.GetAngleToNadir(), 0.001);
}

TEST(EarthSensor, TestCalculateNadirVector) {
    // Data obtained from MATLAB script code of the running algorithm
    double nadir_expected_data_1[3][1] = {
        {-0.404816393349314}, {-0.859347789667761}, {0.4033108072669962}};
    NewStackMatrixMacro(nadir_vector, 3, 1);
    Matrix nadir_expected_1(nadir_expected_data_1);
    EarthSensor earth_sensor;

    earth_sensor.SetPosXSensorReading(0.133315183411290);
    earth_sensor.SetPosYSensorReading(0.004293997614282);
    // earth_sensor.SetPosZSensorReading(0.636557561183074);
    earth_sensor.SetNegXSensorReading(0.595124837677689);
    earth_sensor.SetNegYSensorReading(0.807433740690897);
    earth_sensor.SetNegZASensorReading(0.097213969998926);
    earth_sensor.SetNegZBSensorReading(0.097213969998926);

    earth_sensor.CalculateNadirVector();
    nadir_vector = earth_sensor.GetNadirVector();
    DOUBLES_EQUAL(nadir_expected_1.Get(0, 0), nadir_vector.Get(0, 0), 0.001);
    DOUBLES_EQUAL(nadir_expected_1.Get(1, 0), nadir_vector.Get(1, 0), 0.001);
    DOUBLES_EQUAL(nadir_expected_1.Get(2, 0), nadir_vector.Get(2, 0), 0.001);
}
