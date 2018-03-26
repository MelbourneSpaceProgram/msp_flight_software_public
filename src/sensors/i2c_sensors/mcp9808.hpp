#ifndef SRC_SENSORS_I2C_SENSORS_MCP9808_HPP_
#define SRC_SENSORS_I2C_SENSORS_MCP9808_HPP_

#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <string>

/**
  MCP9808 class.

  This describes a single MCP9808 sensor.
*/
class MCP9808 : public I2cSensor<double> {
   public:
    /**
      MCP9808 constructor.

      @param bus The bus that the sensor is connected to.
      @param address The address of the sensor.
      @param id The unique ID by which the sensor is identified.
      @return The MCP9808 object.
    */
    MCP9808(I2c* bus, int address);

    /**
      Method that causes the MCP9808 to take a reading. The value is then
      stored in the reading variable, and all observers are notified.
      This is an implementation of the virtual function in I2CSensor.

      @return The sensor reading.
    */
    bool TakeReading();

   private:
    /**
      The register in which the MCP9808 temperature is stored.
    */
    static const int TEMP_REGISTER;

    /**
      Method that returns the measured temperature.

      @return The sensor voltage.
    */
    double get_temperature(void);
};

#endif  //  SRC_SENSORS_I2C_SENSORS_MCP9808_HPP_
