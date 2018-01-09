#ifndef SRC_I2C_I2C_BUS_HPP_
#define SRC_I2C_I2C_BUS_HPP_

#include <ti/drivers/I2C.h>
#include <src/i2c/i2c_configuration.hpp>

/**
  I2CBus class.

  This describes the I2C configuration.
*/
class I2CBus {
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
      Function that returns a handle to the open I2C bus. Must be called after
      the I2C_Bus::open method.

      @return The I2C_Handle.
    */
    I2C_Handle get_handle(void);

    /**
      Function that performs a composite I2C read/write transaction and writes
      any response data to the read buffer.

      @param address The address of the I2C device on the bus.
      @param read_buffer The buffer to which the I2C device will write.
      @param read_buffer_length The length of the read buffer.
      @param write_buffer The buffer which will be used to perform the initial
      write to the I2C device.
      @param write_buffer_length The length of the write buffer.
    */
    bool perform_transaction(int address, uint8_t* read_buffer,
                             int read_buffer_length, uint8_t* write_buffer,
                             int write_buffer_length);

    /**
      Function that performs an I2C write operation.

      @param address The address of the I2C device on the bus.
      @param write_buffer The buffer which will be used to perform the initial
      write to the I2C device.
      @param write_buffer_length The length of the write buffer.
    */
    bool perform_write_transaction(int address, uint8_t* write_buffer,
                                   int write_buffer_length);

    /**
      Function that performs an I2C write operation.

      @param address The address of the I2C device on the bus.
      @param read_buffer The buffer to which the I2C device will write.
      @param read_buffer_length The length of the read buffer.
    */
    bool perform_read_transaction(int address, uint8_t* read_buffer,
                                  int read_buffer_length);

   private:
    /**
      The I2C configuration for the bus.
    */
    I2CConfiguration* config;

    /**
      The index of the I2C bus on the board.
    */
    int index;

    /**
      The handle to the open I2C bus.
    */
    I2C_Handle handle;
};

#endif  // SRC_I2C_I2C_BUS_HPP_
