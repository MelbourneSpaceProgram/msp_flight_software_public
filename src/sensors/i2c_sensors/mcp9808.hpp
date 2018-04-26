#ifndef SRC_SENSORS_I2C_SENSORS_MCP9808_HPP_
#define SRC_SENSORS_I2C_SENSORS_MCP9808_HPP_

#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <string>

/**
  MCP9808 class.

  This describes a single MCP9808 sensor.
*/
class MCP9808 : public I2cSensor {
   public:
    /**
      MCP9808 constructor.

      @param bus The bus that the sensor is connected to.
      @param address The address of the sensor.
      @return The MCP9808 object.
    */
    MCP9808(const I2c* bus, int address,
            const I2cMultiplexer* multiplexer = NULL,
            I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    /**
      Method that causes the MCP9808 to take a reading. The value is then
      stored in the reading variable, and all observers are notified.
      This is an implementation of the virtual function in I2CSensor.

      @return The sensor reading.
    */
    float TakeI2cReading();

   private:
    /**
      The register in which the MCP9808 temperature is stored.
    */
    static const int TEMP_REGISTER;

    /**
      Method that returns the measured temperature.

      @return The sensor voltage.
    */
    float get_temperature(void);
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MCP9808_HPP_
