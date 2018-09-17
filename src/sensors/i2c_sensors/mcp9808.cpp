#include <external/etl/exception.h>
#include <src/board/i2c/i2c.h>
#include <src/sensors/i2c_sensors/mcp9808.h>

Mcp9808::Mcp9808(const I2c* bus, uint8_t address,
                 const I2cMultiplexer* multiplexer,
                 I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {}

double Mcp9808::TakeI2cReading() {
    byte read_buffer[2];
    byte write_buffer[1];

    write_buffer[0] = kTempRegister;

    if (!PerformTransaction(address, read_buffer, 2, write_buffer, 1)) {
        etl::exception e("Failed MCP9808 Reading", __FILE__, __LINE__);
        throw e;
    }

    // See MCP9808 datasheet for conversion logic
    int16_t sign = read_buffer[0] & kSignBitMask ? -1 : 1;
    byte upper_byte = read_buffer[0] & kUpperByteMask;
    byte lower_byte = read_buffer[1];

    if (sign == 1) {
        return upper_byte * kUpperByteScale + lower_byte * kLowerByteScale;
    } else {
        return 256 -
               (upper_byte * kUpperByteScale + lower_byte * kLowerByteScale);
    }
}
