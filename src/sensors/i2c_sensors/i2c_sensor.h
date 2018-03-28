#ifndef SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_
#define SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_

#include <src/i2c/i2c.h>
#include <src/i2c/multiplexers/i2c_multiplexer.h>
#include <src/observers/observer.h>
#include <src/sensors/generic_sensor.h>
#include <string>

template <typename T>
class I2cSensor : public GenericSensor<T> {
   public:
    I2cSensor(
        const I2c* bus, uint8_t address,
        const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannels channel = I2cMultiplexer::kMuxNoChannel)
        : bus(bus),
          address(address),
          multiplexer(multiplexer),
          channel(channel) {}

    I2c* GetBus() { return bus; }
    byte GetAddress() { return address; }
    bool TakeReading() {
        try {
            if (multiplexer != NULL) {
                multiplexer->OpenChannel(channel);
            }
            this->reading = TakeI2cReading();  // Throws exception on failure
            if (multiplexer != NULL) {
                multiplexer->CloseChannel(channel);
            }
            // TODO(dingbenjamin): Get time from timekeeper
            RTime current_time = {0, 0, 0, 0, 0, 0};
            this->timestamp = current_time;
            this->NotifyObservers();
            return true;
        } catch (etl::exception e) {
            // TODO(dingbenjamin): Log sensor failure and remedy
            if (multiplexer != NULL) {
                multiplexer->CloseChannel(channel);
            }
            return false;
        }
    }

   protected:
    virtual T TakeI2cReading() = 0;
    const I2c* bus;
    uint8_t address;

   private:
    const I2cMultiplexer* multiplexer;
    I2cMultiplexer::MuxChannels channel;
};

#endif  //  SRC_SENSORS_I2C_SENSORS_I2C_SENSOR_H_
