#ifndef SRC_CONFIG_SENSOR_CONFIG_HPP_
#define SRC_CONFIG_SENSOR_CONFIG_HPP_

#include <map>
#include <src/i2c/sensors/i2c_sensor.hpp>
#include <string>

/**
  SensorConfig class.

  This describes references to all the sensors onboard the satellite. This
  allows multiple tasks to reference the same sensor by code.
*/
class SensorConfig {
   public:
    /**
      This function returns the single instance of the SensorConfig class.
    */
    static SensorConfig* instance(void);

    /**
      This function returns a reference to an I2CSensor by its code.

      @param code The unique code for the sensor.
    */
    I2CSensor* by_code(string code);

    /**
      This function determines if the config has been built yet.
    */
    bool config_built(void);

   private:
    /**
      The SensorConfig constructor. This method is made private to ensure that
      the singleton nature of the class.
    */
    SensorConfig();

    /**
      This function builds all the sensors on the satellite and stores their
      codes and references in the config map.
    */
    void build_config(void);

    /**
      This determines if the config has been built or not.
    */
    bool built;

    /**
      This is the map between the sensor code and a reference to sensor object.
    */
    map<string, I2CSensor*> config;
};

#endif  // SRC_CONFIG_SENSOR_CONFIG_HPP_
