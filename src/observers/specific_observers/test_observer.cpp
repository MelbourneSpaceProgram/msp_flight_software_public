#include <src/observers/specific_observers/test_observer.h>
#include <src/sensors/specific_sensors/test_i2c_sensor.h>

void TestObserver::Update() {
    TestI2cSensor* sensor_with_reading = GetSensorWithReading();
    if (sensor_with_reading != NULL) {
        observed_reading = GetSensorWithReading()->GetReading();
    }
}

int TestObserver::GetObservedReading() const { return observed_reading; }
