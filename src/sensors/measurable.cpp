#include <external/etl/exception.h>
#include <src/observers/observer.h>
#include <src/sensors/measurable.h>

Measurable::Measurable() : new_reading(false) {}

bool Measurable::AddObserver(Observer* observer) {
    if (observers.available()) {
        observers.push_back(observer);
        return true;
    } else {
        etl::exception e("No capacity to add observer", __FILE__, __LINE__);
        throw e;
    }
}

void Measurable::NotifyObservers() {
    new_reading = true;
    for (etl::vector<Observer*, kMaxObservers>::iterator it = observers.begin();
         it != observers.end(); ++it) {
        (*it)->Update();
    }
    new_reading = false;
}

bool Measurable::HasNewReading() { return new_reading; }
