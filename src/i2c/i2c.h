#ifndef SRC_I2C_I2C_H_
#define SRC_I2C_I2C_H_

#include <src/util/data_types.h>
#include <ti/drivers/I2C.h>
#include <src/i2c/i2c_configuration.h>

/**
  I2c class.

  This describes the I2C Bus.
*/
class I2c {
   public:
    /**
      I2c constructor.

      @param config The I2cConfiguration used by the bus.
      @param index The index of the I2C bus on the board.
      @return The I2c object.
    */
    I2c(I2cConfiguration config, uint8_t index);

    /**
      I2c destructor, closes the I2C bus
    */
    ~I2c();

    /**
      Function that returns a handle to the open I2C bus. Must be called after
      the I2c::open method.

      @return The I2C_Handle.
    */
    I2C_Handle GetHandle() const;

    /**
      Returns a constant reference to the configuration of the I2C bus

      @return The I2C Configuration
    */
    const I2cConfiguration& GetConfig() const;

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
    bool PerformTransaction(byte address, byte* read_buffer,
                             uint16_t read_buffer_length, byte* write_buffer,
                             uint16_t write_buffer_length) const;

    /**
      Function that performs an I2C write operation.

      @param address The address of the I2C device on the bus.
      @param write_buffer The buffer which will be used to perform the initial
      write to the I2C device.
      @param write_buffer_length The length of the write buffer.
    */
    bool PerformWriteTransaction(byte address, byte* write_buffer,
                                 uint16_t write_buffer_length) const;

    /**
      Function that performs an I2C read operation.

      @param address The address of the I2C device on the bus.
      @param read_buffer The buffer to which the I2c device will read.
      @param read_buffer_length The length of the read buffer.
    */
    bool PerformReadTransaction(byte address, byte* read_buffer,
                                uint16_t read_buffer_length) const;

   private:
    /**
      Private copy constructor to ensure no double deletion
    */
    I2c(const I2c&);

    /**
      Private assignment operator to ensure no double deletion
    */
    I2c& operator=(const I2c&);

    /**
      Function that opens the I2C bus.
    */
    void Open();

    /**
      Function that closes the I2C bus.
    */
    void Close();

    /**
      The I2C configuration for the bus.
    */
    I2cConfiguration config;

    /**
      The index of the I2C bus on the board.
    */
    const uint8_t index;

    /**
      The handle to the open I2C bus.
    */
    I2C_Handle handle;
};

#endif  // SRC_I2C_I2C_H_
