#include <src/observers/high_temp_observer.hpp>

void HighTempObserver::update(I2CSensor* sensor) {
  double reading = sensor->get_reading();

  int i = 20;
  int a = 3;

  int x = 20 / 3;
}
