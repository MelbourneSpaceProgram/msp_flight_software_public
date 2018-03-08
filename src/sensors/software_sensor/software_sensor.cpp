#include <src/sensors/software_sensor/software_sensor.h>

SoftwareSensor::SoftwareSensor(std::string id) : id(id) {}

void SoftwareSensor::LoadReading(double reading) {
    initialised = true;
    this->reading = reading;
}

bool SoftwareSensor::TakeReading() {
    if (initialised) {
        NotifyObservers();
    }
    bool reading_successful = initialised;
    initialised = false;
    return reading_successful;
}
