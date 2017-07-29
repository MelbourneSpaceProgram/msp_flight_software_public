/**
  mcp9808.h
  Purpose: Reads values from a MCP9808 sensor.

  @author Brett Clark
  @version 1.0
  @date 15/5/2017
*/

#ifndef _MCP9808_H_
#define _MCP9808_H_

#include "i2c_sensor.h"
#include "string"
#include "iostream"

using namespace std;

/**
  MCP9808 class.

  This describes a single MCP9808 sensor.
*/
class MCP9808 : public I2CSensor {

/**
  Public members of the MCP9808 class.
*/
public:

  /**
    MCP9808 constructor.

    @param bus The bus that the sensor is connected to.
    @param address The address of the sensor.
    @return The MCP9808 object.
  */
  MCP9808(I2CBus* bus, int address);

  /**
    Method that returns the MCP9808 reading.
    This is an implementation of the virtual function in I2CSensor.

    @return The sensor reading.
  */
  double get_reading(void);

/**
  Private members of the MCP9808 class.
*/
private:

  /**
    Constants of the MCP9808 class.
  */

  /**
    The register in which the MCP9808 temperature is stored.
  */
  static const int TEMP_REGISTER;

  /**
    Member variables of the MCP9808 class.
  */

  /**
    Member functions of the MCP9808 class.
  */

  /**
    Method that returns the measured temperature.

    @return The sensor voltage.
  */
  double get_temperature(void);

};

#endif // _MCP9808_H_
