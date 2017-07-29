/**
  i2c_sensor.h
  Purpose: Abstract class containing shared functionality of all I2C sensors.

  @author Brett Clark
  @version 1.0
  @date 3/5/2017
*/

#ifndef _I2CSensor_H_
#define _I2CSensor_H_

#include <src/i2c/i2c_bus.hpp>

/**
  I2CSensor class.

  This base class describes a I2CSensor.
*/
class I2CSensor {

/**
  Public members of the I2CSensor class.
*/
public:

  /**
    I2CSensor constructor.

    @param bus The bus that the sensor is attached to.
    @param address The address of the sensor.
    @return The I2CSensor object.
  */
  I2CSensor(I2CBus* bus, int address);

  /**
    Method that returns a reference to the sensor bus.

    @return A reference to the sensor bus.
  */
  I2CBus *get_bus(void);

  /**
    Method that returns the sensor address.

    @return The sensor address.
  */
  int get_address(void);

  /**
    Virtual public members of the I2CSensor class.
  */

  /**
    Method that returns the sensor reading.

    @return The sensor reading. 
  */
  virtual double get_reading(void) = 0;

/**
  Private members of the I2CSensor class.
*/
private:

  /**
    Member variables of the I2CSensor class.
  */

  /**
    A reference to the bus that the sensor is connected to.
  */
  I2CBus* bus;

  /**
    The address of the sensor.
  */
  int address;
};

#endif // _I2CSensor_H_
