#include <src/observers/low_temp_observer.hpp>
#include <Board.h>
#include <ti/drivers/GPIO.h>

void LowTempObserver::update(I2CSensor* sensor) {
  double reading = sensor->get_reading();

  if (reading < 27.0) {
    GPIO_write(Board_LED3, 1);
  } else {
    GPIO_write(Board_LED3, 0);
  }
}
