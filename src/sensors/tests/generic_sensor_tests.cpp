#include <CppUTest/TestHarness.h>
#include <src/observers/specific_observers/test_observer.h>
#include <src/sensors/test_sensors/test_int_sensor.h>

TEST_GROUP(GenericSensor){};

TEST(GenericSensor, TestGenericSensor) {
    TestObserver observer;
    TestIntSensor test_i2c_sensor;

    uint8_t dummy_reading = 1;

    observer.RegisterWithSensor(&test_i2c_sensor);

    test_i2c_sensor.SetDummySensorData(dummy_reading);
    test_i2c_sensor.TakeReading();

    CHECK_EQUAL(dummy_reading, observer.GetObservedReading());
}
