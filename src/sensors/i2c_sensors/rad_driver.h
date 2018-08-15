/*
 * rad_driver.h
 * I2C Radiation Driver
 */

#ifndef I2C_SENSORS_RAD_DRIVER_H_
#define I2C_SENSORS_RAD_DRIVER_H_

#include <src/board/i2c/io_expander/io_expander.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/util/task_utils.h>
#include <string>

class RadDriver : public I2cDevice {
   public:
    /**
      RadDriver constructor.

      @param bus The bus that the sensor is connected to.
      @param address The address of the sensor.
      @return The RadDriver object.
    */
    RadDriver(
        const I2c* bus, int address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    /**
      Method that causes the RADIATIONDRIVER to take a reading. The value is
      then stored in the reading variable, and all observers are notified. This
      is an implementation of the virtual function in I2cDevice.

      @return The sensor reading.
    */
    double TakeI2cReading();

   private:
    static const int kRadAddress = 0x20;
    static const int kSamplePeriodTime = 250;  // sampling time (milliseconds)
    static const int kWaitTime = 1;            // (milliseconds)
};

#endif /* I2C_SENSORS_RAD_DRIVER_H_ */
