/**
  tmp006.h
  Purpose: Reads values from a TMP006 sensor.

  @author Brett Clark
  @version 1.0
  @date 3/5/2017
*/

#ifndef _TMP006_H_
#define _TMP006_H_

#include "i2c_sensor.h"
#include "string"
#include "iostream"


using namespace std;

/**
  TMP006 class.

  This describes a single TMP006 sensor.
*/
class TMP006 : public I2CSensor {

/**
  Public members of the TMP006 class.
*/
public:

  /**
    TMP006 constructor.

    @param bus The bus that the sensor is connected to.
    @param address The address of the sensor.
    @param constant The calibration constant for the sensor.
    @return The TMP006 object.
  */
  TMP006(I2CBus* bus, int address, double constant);

  /**
    Method that returns the TMP006 reading. 
    This is an implementation of the virtual function in I2CSensor.

    @return The sensor reading.
  */
  double get_reading(void);

/**
  Private members of the TMP006 class.
*/
private:

  /**
    Constants of the TMP006 class.
  */

  /**
    The register in which the TMP006 sensor voltage is stored.
  */
  static const int SENSOR_VOLTAGE_REGISTER;

  /**
    The register in which the TMP006 die temperature is stored.
  */
  static const int DIE_TEMP_REGISTER;

  /**
    Member variables of the TMP006 class.
  */

  /**
    The calibration constant of the I2CSensor.
  */
  double constant;

  /**
    Member functions of the TMP006 class.
  */

  /**
    Method that returns the sensor voltage.

    @return The sensor voltage.
  */
  double get_sensor_voltage(void);

  /**
    Method that returns the die temperature.

    @return The die temperature.
  */
  double get_die_temperature(void);

  /**
    Method that converts the sensor and die temperatures into an object temperature.

    @param sensor_voltage The voltage reading from the sensor.
    @param die_temperature The temperature of the die.
    @return The object temperature.
  */
  double get_object_temperature(double sensor_voltage, double die_temperature);
};

#endif // _TMP006_H_
