#include <src/observers/high_temp_observer.hpp>
#include <Board.h>
#include <ti/drivers/GPIO.h>

void HighTempObserver::update(I2CSensor* sensor) {
  double reading = sensor->get_reading();

  if (reading > 27.0) {
    GPIO_write(Board_LED1, 1);
  } else {
    GPIO_write(Board_LED1, 0);
  }
}
