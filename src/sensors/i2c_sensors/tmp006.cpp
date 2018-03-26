#include <src/sensors/i2c_sensors/tmp006.hpp>

#include <cmath>

/**
  Constants of the TMP006 class.
*/

const int TMP006::SENSOR_VOLTAGE_REGISTER = 0x00;
const int TMP006::DIE_TEMP_REGISTER = 0x01;

/**
  Public methods of the TMP006 class.
*/

TMP006::TMP006(I2c* bus, int address, double constant)
    : I2cSensor<double>(bus, address), constant(constant) {}

bool TMP006::TakeReading(void) {
    uint8_t read_buffer[2];
    uint8_t write_buffer[1];

    write_buffer[0] = TMP006::DIE_TEMP_REGISTER;

    bool reading_successful = bus->PerformTransaction(
        this->GetAddress(), read_buffer, 2, write_buffer, 1);
    double temperature = (read_buffer[0] << 6) | (read_buffer[1] >> 2);
    temperature /= 32.0;
    reading = temperature;

    if (reading_successful) {
        NotifyObservers();
    }

    return reading_successful;
}

/**
  Private methods of the TMP006 class.
*/

double TMP006::get_sensor_voltage(void) { return 3.5; }

double TMP006::get_object_temperature(double sensor_voltage,
                                      double die_temperature) {
    double object_temp_power_4 =
        std::pow(die_temperature, 4) + (sensor_voltage / this->constant);

    // Handle negative reading.
    if (object_temp_power_4 < 0) {
        return NULL;
    }

    return std::pow(object_temp_power_4, 1.0 / 4);
}
