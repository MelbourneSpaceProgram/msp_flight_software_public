#include <Board.h>
#include <src/observers/specific_observers/sample_temp_observer.hpp>
#include <src/sensors/test_sensors/test_i2c_sensor.h>
#include <ti/drivers/GPIO.h>
#include <src/observers/specific_observers/sample_temp_observer.hpp>

void SampleTempObserver::Update() {
    double reading = 0;
    if (GetSensorWithReading() != NULL) {
        reading = GetSensorWithReading()->GetReading();
    }
}
