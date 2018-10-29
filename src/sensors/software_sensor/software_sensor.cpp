#include <src/sensors/software_sensor/software_sensor.h>

SoftwareSensor::SoftwareSensor(std::string id) : id(id) {}

void SoftwareSensor::LoadReading(double reading) {
    initialised = true;
    last_reading = reading;
}

double SoftwareSensor::GetReading() {
    if (initialised) {
        initialised = false;
        return last_reading;
    } else {
        return 0;
    }
}
