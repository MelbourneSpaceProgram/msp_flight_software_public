#include <src/observers/basic_fault_observer.hpp>

void BasicFaultObserver::update(I2CSensor* sensor) {
    double reading = sensor->get_reading();

    int i = 20;
    int a = 3;

    int x = 20 / 3;
}
