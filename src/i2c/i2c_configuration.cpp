#include <src/i2c/i2c_configuration.h>

I2cConfiguration::I2cConfiguration() { I2C_Params_init(&i2c_params); }

I2cConfiguration::I2cConfiguration(I2C_BitRate bit_rate) {
    I2C_Params_init(&i2c_params);
    i2c_params.bitRate = bit_rate;
}

I2cConfiguration::I2cConfiguration(I2C_Params I2C_params)
    : i2c_params(i2c_params) {}

const I2C_Params& I2cConfiguration::GetI2cParams() const {
    return i2c_params;
}
