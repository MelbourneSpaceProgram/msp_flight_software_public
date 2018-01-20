#ifndef SRC_I2C_SENSORS_I2C_SENSOR_HPP_
#define SRC_I2C_SENSORS_I2C_SENSOR_HPP_

#include <src/i2c/i2c.h>
#include <string>
#include <vector>
#include <src/observers/observer.hpp>

// Forward declarations.
class Observer;

/**
  I2CSensor class.

  This base class describes an I2CSensor.
*/
class I2CSensor {
public:
  /**
    I2CSensor constructor.

    @param bus The bus that the sensor is attached to.
    @param address The address of the sensor.
    @param id The unique ID for identifying the sensor.
    @return The I2CSensor object.
  */
  I2CSensor(I2c* bus, int address, std::string id);

  /**
    Method that returns a reference to the sensor bus.

    @return A reference to the sensor bus.
  */
  I2c *get_bus(void);

  /**
    Method that returns the sensor address.

    @return The sensor address.
  */
  int get_address(void);

  /**
    Method that returns the sensor ID.

    @return The unique ID by which the sensor is identified.
  */
  std::string get_id(void);

  /**
    Method that returns the sensor reading.

    @return The sensor reading.
   */
  double get_reading(void);

  /**
    Method that adds an observer to the list.

    @param observer A class that is interested in the sensor reading.
   */
  void add_observer(Observer* observer);

  /**
    Virtual method that should be overwritten by any derived classes.
    This method should take a sensor reading and call the notify_observers
    method to notify all observers in the list.
  */
  virtual void take_reading(void) = 0;

protected:
  /**
    A reference to the bus that the sensor is connected to.
  */
  I2c* bus;

  /**
    The address of the sensor.
  */
  int address;

  /**
    The unique ID used to identify the sensor.
  */
  std::string id;

  /**
    The sensor reading.
  */
  double reading;

  /**
    The list of observers.
  */
  std::vector<Observer*> observers;

  /**
    Method that notifies all attached observers that the sensor
    reading has been taken.s
   */
  void notify_observers(void);
};

#endif  //  SRC_I2C_SENSORS_I2C_SENSOR_HPP_
