/**
  mcp9808.hpp
  Purpose: Reads values from a MCP9808 sensor.

  @author Brett Clark
  @version 1.0
  @date 15/5/2017
*/

#ifndef _MCP9808_HPP_
#define _MCP9808_HPP_

#include <src/i2c/sensors/i2c_sensor.hpp>

using namespace std;

/**
  MCP9808 class.

  This describes a single MCP9808 sensor.
*/
class MCP9808 : public I2CSensor {

public:

  /**
    MCP9808 constructor.

    @param bus The bus that the sensor is connected to.
    @param address The address of the sensor.
    @param id The unique ID by which the sensor is identified.
    @return The MCP9808 object.
  */
  MCP9808(I2CBus* bus, int address, string id);

  /**
    Method that causes the MCP9808 to take a reading. The value is then
    stored in the reading variable, and all observers are notified.
    This is an implementation of the virtual function in I2CSensor.

    @return The sensor reading.
  */
  void take_reading(void);

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

#endif // _MCP9808_HPP_
