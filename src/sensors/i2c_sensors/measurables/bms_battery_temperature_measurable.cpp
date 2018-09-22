#include <src/sensors/i2c_sensors/measurables/bms_battery_temperature_measurable.h>

BmsBatteryTemperatureMeasurable::BmsBatteryTemperatureMeasurable(
    Bms* temp_sensor)
    : I2cMeasurable<TemperatureReading>(temp_sensor,
                                        TemperatureReading_init_default) {}

TemperatureReading BmsBatteryTemperatureMeasurable::TakeDirectI2cReading() {
    TemperatureReading reading = TemperatureReading_init_default;
    Bms* temp_sensor = static_cast<Bms*>(I2cMeasurable::sensor);
    reading.temp = temp_sensor->GetBatteryTemp();
    return reading;
}
