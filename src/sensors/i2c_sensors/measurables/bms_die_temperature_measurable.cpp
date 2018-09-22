#include <src/sensors/i2c_sensors/measurables/bms_die_temperature_measurable.h>

BmsDieTemperatureMeasurable::BmsDieTemperatureMeasurable(Bms* temp_sensor)
    : I2cMeasurable<TemperatureReading>(temp_sensor,
                                        TemperatureReading_init_default) {}

TemperatureReading BmsDieTemperatureMeasurable::TakeDirectI2cReading() {
    Bms* temp_sensor = static_cast<Bms*>(I2cMeasurable::sensor);
    TemperatureReading reading = TemperatureReading_init_default;
    reading.temp = temp_sensor->GetDieTemp();
    return reading;
}
