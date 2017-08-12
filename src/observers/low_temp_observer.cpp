#include <src/observers/low_temp_observer.hpp>

void LowTempObserver::update(I2CSensor* sensor) {
  double reading = sensor->get_reading();

  int i = 20;
  int a = 3;

  int x = 20 / 3;
}
