#include <src/sensors/software_sensor/software_sensor.h>

SoftwareSensor::SoftwareSensor(std::string id) : id(id) {}

void SoftwareSensor::SetReading(double reading) { this->reading = reading; }

void SoftwareSensor::TakeReading() {
    new_reading = true;
    NotifyObservers();
    new_reading = false;
}
