#include <src/config/satellite.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/util/msp_exception.h>
#include <cstdio>

GateMutexPri_Params mutex_params;
GateMutexPri_Handle I2cDevice::i2c_mutex = NULL;

I2cDevice::I2cDevice(const I2c* bus, uint8_t address,
                     const I2cMultiplexer* multiplexer,
                     I2cMultiplexer::MuxChannel channel)
    : address(address),
      bus(bus),
      multiplexer(multiplexer),
      channel(channel),
      info_string(kInfoStringLength, ' ') {
    if (i2c_mutex == NULL) {
        GateMutexPri_Params_init(&mutex_params);
        i2c_mutex = GateMutexPri_create(&mutex_params, NULL);
        if (i2c_mutex == NULL) {
            throw MspException("Failed to create mutex.", kI2cDeviceMutexFail,
                               __FILE__, __LINE__);
        }
    }

    // Create info string for adding to logs
    char temp_string[kInfoStringLength];
    snprintf(temp_string, sizeof(temp_string),
             "sensor 0x%02x on bus %c (mux line "
             "0x%02x)",
             address, bus->GetBusLabel(), channel);
    info_string.insert(0, temp_string, kInfoStringLength);
}

bool I2cDevice::MuxSelect() const {
    if (multiplexer != NULL && SystemConfiguration::GetInstance()->IsI2cAvailable()) {
        if (!multiplexer->CloseAllChannels()) {
            Log_error2("Unable to contact mux on bus %d for sensor address %d",
                       multiplexer->GetBus()->index, address);
            return false;
        }
        if (!multiplexer->OpenChannel(channel)) return false;
    }
    return true;
}

bool I2cDevice::MuxDeselect() const {
    if (multiplexer != NULL && SystemConfiguration::GetInstance()->IsI2cAvailable()) {
        if (!multiplexer->CloseAllChannels()) return false;
    }
    return true;
}

const I2c* I2cDevice::GetI2cBus() const { return bus; }

uint8_t I2cDevice::GetI2cAddress() const { return address; }

const I2cMultiplexer* I2cDevice::GetI2cMultiplexer() const {
    return multiplexer;
}

I2cMultiplexer::MuxChannel I2cDevice::GetMultiplexerChannel() const {
    return channel;
}

std::string I2cDevice::GetInfoString() const { return info_string; }

bool I2cDevice::PerformWriteTransaction(byte address, byte* write_buffer,
                                        uint16_t write_buffer_length) const {
    MutexLocker locker(i2c_mutex);
    bool success;
    if (!MuxSelect()) return false;
    success = bus->PerformWriteTransaction(address, write_buffer,
                                           write_buffer_length);
    if (!MuxDeselect()) return false;
    return success;
}

bool I2cDevice::PerformReadTransaction(byte address, byte* write_buffer,
                                       uint16_t write_buffer_length) const {
    MutexLocker locker(i2c_mutex);
    bool success;
    if (!MuxSelect()) return false;
    success =
        bus->PerformReadTransaction(address, write_buffer, write_buffer_length);
    if (!MuxDeselect()) return false;
    return success;
}

bool I2cDevice::PerformTransaction(byte address, byte* read_buffer,
                                   uint16_t read_buffer_length,
                                   byte* write_buffer,
                                   uint16_t write_buffer_length) const {
    MutexLocker locker(i2c_mutex);
    bool success;
    if (!MuxSelect()) return false;
    success = bus->PerformTransaction(address, read_buffer, read_buffer_length,
                                      write_buffer, write_buffer_length);
    if (!MuxDeselect()) return false;
    return success;
}
