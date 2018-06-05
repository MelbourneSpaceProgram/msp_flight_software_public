#include <external/etl/exception.h>
#include <src/board/i2c/i2c.h>
#include <src/sensors/i2c_sensors/mcp9808.hpp>
#include <string>

const int MCP9808::TEMP_REGISTER = 0x05;

MCP9808::MCP9808(const I2c* bus, int address, const I2cMultiplexer* multiplexer,
                 I2cMultiplexer::MuxChannel channel)
    : I2cSensor(bus, address, multiplexer, channel) {}

double MCP9808::TakeI2cReading() {
    uint8_t read_buffer[2];
    uint8_t write_buffer[1];

    // Tell the MCP9808 to read from the temperature register.
    write_buffer[0] = MCP9808::TEMP_REGISTER;

    // Perform I2C transaction.
    bool reading_successful =
        bus->PerformTransaction(address, read_buffer, 2, write_buffer, 1);

    // Perform the necessary bit-shifting on the read buffer.
    double temperature = ((read_buffer[0] & ~(7 << 5)) << 8) | (read_buffer[1]);

    temperature /= 16.0;

    if (reading_successful) {
        return temperature;
    } else {
        etl::exception e("Failed MCP9808 Reading", __FILE__, __LINE__);
        throw e;
    }
}
