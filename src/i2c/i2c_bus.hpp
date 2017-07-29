/**
  i2c_bus.h
  Purpose: Represents an I2C bus.

  @author Brett Clark
  @version 1.0
  @date 3/5/2017
*/

#ifndef _I2C_BUS_H_
#define _I2C_BUS_H_

#include "i2c_configuration.h"
#include "ti/drivers/I2C.h"

/**
  I2CBus class.

  This describes the I2C configuration.
*/
class I2CBus {

/**
  Public members of the I2CBus class.
*/
public:

  /**
    I2CBus constructor.

    @param config The I2CConfiguration used by the bus.
    @param index The index of the I2C bus on the board.
    @return The I2CBus object.
  */
  I2CBus(I2CConfiguration* config, int index);

  /**
    Function that opens the I2C bus.
  */
  void open(void);

  /**
    Function that closes the I2C bus.
  */
  void close(void);

  /**
    Function that returns a handle to the open I2C bus. Must be called after the I2C_Bus::open method.

    @return The I2C_Handle.
  */
  I2C_Handle get_handle(void);

  /**
     Function that performs an I2C Transaction and writes any response data to the read buffer.

     @param address The address of the I2C device on the bus.
     @param read_buffer The buffer to which the I2C device will write.
     @param read_buffer_length The length of the read buffer.
     @param write_buffer The buffer which will be used to perform the initial write to the I2C device.
     @param write_buffer_length The length of the write buffer.
   */
  void perform_transaction(int address, uint8_t* read_buffer, int read_buffer_length, uint8_t* write_buffer, int write_buffer_length);
  void perform_write_transaction(int address, uint8_t* write_buffer, int write_buffer_length);
  void perform_read_transaction(int address, uint8_t* read_buffer, int read_buffer_length);
  /**
  Private members of the I2CBus class.
*/
private:

  /**
    Member variables of the I2CBus class.
  */

  /**
    The I2C configuration for the bus.
  */
  I2CConfiguration *config;

  /**
    The index of the IC bus on the board.
  */
  int index;

  /**
    The handle to the open I2C bus.
  */
  I2C_Handle handle;
};

#endif // _I2C_BUS_H_
