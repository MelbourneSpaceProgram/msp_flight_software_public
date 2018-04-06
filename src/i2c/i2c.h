#ifndef SRC_I2C_I2C_H_
#define SRC_I2C_I2C_H_

#include <external/etl/array.h>
#include <src/util/data_types.h>
#include <ti/drivers/I2C.h>

/**
  I2c class.

  This describes the I2C Bus.
*/
class I2c {
   public:
    static const I2C_BitRate kBitRate100K = I2C_100kHz;
    static const I2C_BitRate kBitRate400K = I2C_400kHz;
    static const uint16_t kTimeoutMilliSeconds = 500;
    /**
      I2c constructor.
      Opens I2C bus with timeout at 100kHz

      @param index The index of the I2C bus on the board.
      @return The I2c object.
    */
    explicit I2c(uint8_t index);

    /**
      I2c constructor.
      Opens I2C bus with timeout at specified bit rate

      @param bit_rate The bit rate of the I2C bus.
      @param index The index of the I2C bus on the board.
      @return The I2c object.
    */
    I2c(I2C_BitRate bit_rate, uint8_t index);

    /**
      Function that returns a handle to the open I2C bus. Must be called after
      the I2c::open method.

      @return The I2C_Handle.
    */
    I2C_Handle GetHandle() const;

    /**
      Returns a copy  of the parameters of the I2C bus

      @return The I2C Parameters
    */
    I2C_Params GetI2cParams() const;

    /**
      Function that performs a composite I2C read/write transaction and writes
      any response data to the read buffer. The tranfer is performed in callback
      mode and which provides timeout functionality. The timeout is set by
      kTimeoutMilliSeconds.

      @param address The address of the I2C device on the bus.
      @param read_buffer The buffer to which the I2C device will write.
      @param read_buffer_length The length of the read buffer.
      @param write_buffer The buffer which will be used to perform the initial
      write to the I2C device.
      @param write_buffer_length The length of the write buffer.
      @return The outcome of the transfer with success returning true.
    */
    bool PerformTransaction(byte address, byte* read_buffer,
                             uint16_t read_buffer_length, byte* write_buffer,
                             uint16_t write_buffer_length) const;

    /**
      Function that performs an I2C write operation with timeout.

      @param address The address of the I2C device on the bus.
      @param write_buffer The buffer which will be used to perform the initial
      write to the I2C device.
      @param write_buffer_length The length of the write buffer.
      @return The outcome of the transfer with success returning true.
    */
    bool PerformWriteTransaction(byte address, byte* write_buffer,
                                 uint16_t write_buffer_length) const;

    /**
      Function that performs an I2C read operation with timeout.

      @param address The address of the I2C device on the bus.
      @param read_buffer The buffer to which the I2c device will read.
      @param read_buffer_length The length of the read buffer.
      @return The outcome of the transfer with success returning true.
    */
    bool PerformReadTransaction(byte address, byte* read_buffer,
                                uint16_t read_buffer_length) const;
    static void InitBusses();

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
      Sets I2C_params for callback mode

      @param callback_function A pointer to a callback function
      @return void
    */
    void SetCallbackMode(I2C_CallbackFxn callback_function);

    /**
       The callback function used when the I2c bus is in callback mode.
       The callback function is given the handle of the bus, the transaction
       struct and the outcome of the transfer.

       @param handle The I2C_Handle of the peripheral used in the transfer.
       @param i2c_transaction The I2C_Transaction struct used for the transfer.
       @param success The outcome of the transfer.
       @return void
    */
    static void ManageI2cTimeout(I2C_Handle handle,
                                    I2C_Transaction *i2c_transaction,
                                    bool success);

    /**
      The parameters for the I2C bus
   */
    I2C_Params i2c_params;

    /**
      The index of the I2C bus on the board.
    */
    const uint8_t index;

    /**
      The handle to the open I2C bus.
    */
    I2C_Handle handle;

    static etl::array<I2C_Handle, 4> I2c_busses;
    static etl::array<I2C_Params, 4> I2c_params;
};

#endif  // SRC_I2C_I2C_H_
