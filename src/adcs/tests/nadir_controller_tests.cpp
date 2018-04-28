//
//#include <math.h>
//#include <src/adcs/controllers/nadir_controller.h>
////#include <test_runners/nadir_controller_tests.h>
//#include <test_runners/unity.h>
//
//void TestNadirControl()
//{
//    double error_quat_data[4][1] = {{0.0603},    {0.8606},    {0.2338},    {0.3798}};
//
//    double omega_data[3][1] = {{-0.5312},   {-0.5863},   {-0.9253}};
//
//    double cData[3][1];
//
//    Matrix error_quat(error_quat_data), omega(omega_data), torque(cData);
//    NadirController::Control(error_quat, omega ,torque);
//
//    double quat1 = torque.Get(0,0);
//    double quat2 = torque.Get(1,0);
//    double quat3 = torque.Get(2,0);
//
//    TEST_ASSERT_DOUBLE_WITHIN(0.01, 3.7008 ,quat1);
//    TEST_ASSERT_DOUBLE_WITHIN(0.01, 4.0995, quat2);
//    TEST_ASSERT_DOUBLE_WITHIN(0.01, 6.4693, quat3);
//
//
//
//}
//
//
//
//
