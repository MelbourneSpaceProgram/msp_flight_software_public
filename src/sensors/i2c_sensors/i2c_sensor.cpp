#include <src/sensors/i2c_sensors/i2c_sensor.h>

I2cSensor::I2cSensor(const I2c* bus, uint8_t address,
                     const I2cMultiplexer* multiplexer,
                     I2cMultiplexer::MuxChannel channel)
    : bus(bus), address(address), multiplexer(multiplexer), channel(channel) {}

void I2cSensor::MuxSelect() const {
    if (multiplexer != NULL) {
        multiplexer->OpenChannel(channel);
    }
}

void I2cSensor::MuxDeselect() const {
    if (multiplexer != NULL) {
        multiplexer->CloseChannel(channel);
    }
}
