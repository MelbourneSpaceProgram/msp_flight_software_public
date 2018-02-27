#include <src/observers/specific_observers/test_observer.h>
#include <test_runners/generic_sensor_tests.h>
#include <test_runners/unity.h>
#include <src/sensors/specific_sensors/test_int_sensor.h>

void TestGenericSensor(void) {
    TestObserver observer;
    TestIntSensor test_i2c_sensor("test");

    uint8_t dummy_reading = 1;

    observer.RegisterWithSensor(&test_i2c_sensor);

    test_i2c_sensor.SetDummySensorData(dummy_reading);
    test_i2c_sensor.TakeReading();

    TEST_ASSERT_EQUAL_UINT8(dummy_reading, observer.GetObservedReading());
}
