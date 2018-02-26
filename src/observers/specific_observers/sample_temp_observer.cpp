#include <Board.h>
#include <src/observers/specific_observers/sample_temp_observer.hpp>
#include <src/sensors/specific_sensors/test_i2c_sensor.h>
#include <ti/drivers/GPIO.h>

void SampleTempObserver::Update() {
    double reading = 0;
    if (GetSensorWithReading() != NULL) {
        reading = GetSensorWithReading()->GetReading();
    }

    if (reading > 27.0) {
        GPIO_write(Board_LED1, 1);
        GPIO_write(Board_LED2, 0);
        GPIO_write(Board_LED3, 0);
    } else if (reading > 25.0) {
        GPIO_write(Board_LED1, 0);
        GPIO_write(Board_LED2, 1);
        GPIO_write(Board_LED3, 0);
    } else {
        GPIO_write(Board_LED1, 0);
        GPIO_write(Board_LED2, 0);
        GPIO_write(Board_LED3, 1);
    }
}
