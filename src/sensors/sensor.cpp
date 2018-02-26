#include <src/observers/observer.h>
#include <src/sensors/sensor.h>

Sensor::Sensor() : new_reading(false) {}

bool Sensor::AddObserver(Observer* observer) {
    if (observers.available()) {
        observers.push_back(observer);
        return true;
    } else {
        // TODO(wschuetz) Confirm exception handling mechanism. Can remove
        //                if exceptions are enabled.
        return false;
    }
}

void Sensor::NotifyObservers() {
    for (etl::vector<Observer*, kMaxObservers>::iterator it = observers.begin();
         it != observers.end(); it++) {
        (*it)->Update();
    }
}

bool Sensor::HasNewReading() {
    return new_reading;
}
