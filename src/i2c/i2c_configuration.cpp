#include <src/i2c/i2c_configuration.hpp>
#include "iostream"

using namespace std;

I2CConfiguration::I2CConfiguration(int transfer_mode, int bit_rate) : transfer_mode(transfer_mode), bit_rate(bit_rate) {}

int I2CConfiguration::get_transfer_mode() {
    return this->transfer_mode;
}

int I2CConfiguration::get_bit_rate() {
    return this->bit_rate;
}
