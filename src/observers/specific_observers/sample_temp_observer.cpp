#include <src/board/board.h>
#include <src/observers/specific_observers/sample_temp_observer.hpp>
#include <src/sensors/test_sensors/test_i2c_measurable.h>
#include <ti/drivers/GPIO.h>

void SampleTempObserver::Update() {
    if (GetSensorWithReading() != NULL) {
         double reading = GetSensorWithReading()->GetReading();
    }
}
