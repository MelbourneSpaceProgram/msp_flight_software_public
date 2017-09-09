#include <src/observers/sample_temp_observer.hpp>
#include <Board.h>
#include <ti/drivers/GPIO.h>

void SampleTempObserver::update(I2CSensor* sensor) {
  double reading = sensor->get_reading();

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
