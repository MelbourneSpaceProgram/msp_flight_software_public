#ifndef SRC_I2C_I2C_CONFIGURATION_H_
#define SRC_I2C_I2C_CONFIGURATION_H_

#include <src/util/data_types.h>
#include <ti/drivers/I2C.h>

/**
  I2cConfiguration class.

  This describes the I2C configuration.
*/
class I2cConfiguration {
   public:
    static const I2C_BitRate kBitRate100K = I2C_100kHz;
    static const I2C_BitRate kBitRate400K = I2C_400kHz;

    /**
      I2cConfiguration default constructor.

      @return The I2cConfiguration object.
    */
    I2cConfiguration();

    /**
      I2cConfiguration constructor.

      @param bit_rate The bit rate of the I2C
      @return The I2cConfiguration object.
    */
    explicit I2cConfiguration(I2C_BitRate bit_rate);

    /**
      I2cConfiguration constructor that takes a I2C_Params arg

      @param I2C_params The parameters for the I2cConfiguration
      @return The I2cConfiguration object.
    */
    explicit I2cConfiguration(I2C_Params I2C_params);

    /**
      Returns the I2C Parameters

      @return The I2C Parameters
    */
    const I2C_Params& GetI2cParams() const;

   private:
    /**
      The parameters for the I2cConfiguration
    */
    I2C_Params i2c_params;
};

#endif  //  SRC_I2C_I2C_CONFIGURATION_H_
