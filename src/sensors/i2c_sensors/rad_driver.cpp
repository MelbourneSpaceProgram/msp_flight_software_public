/*
 * rad_driver.cpp
 * I2C Radiation Driver
 *
 */

#include <external/etl/exception.h>
#include <src/board/i2c/i2c.h>
#include <src/sensors/i2c_sensors/rad_driver.h>

RadDriver::RadDriver(const I2c* bus, int address,
                     const I2cMultiplexer* multiplexer,
                     I2cMultiplexer::MuxChannel channel)
    : I2cDevice(bus, address, multiplexer, channel) {}

double RadDriver::TakeI2cReading() {
    // Carry out radiation measurement algorithm:
    I2cIoExpander io_expander(I2cDevice::GetI2cBus(), RadDriver::kRadAddress);

    // Setting all IO pins in the range 0-11 to input, and IO pin 12 to output
    io_expander.RangeSetDirection(I2cIoExpander::kIoPin0,
                                  I2cIoExpander::kIoPin11,
                                  I2cIoExpander::kIoInput);
    io_expander.SetDirection(I2cIoExpander::kIoPin12, I2cIoExpander::kIoOutput);

    // 1 - Set rst pin high
    io_expander.SetPin(I2cIoExpander::kIoPin12, true);

    // 2 - Wait > 5ns
    TaskUtils::SleepMilli(RadDriver::kWaitTime);

    // 3 - Set rst pin low
    io_expander.SetPin(I2cIoExpander::kIoPin15, false);

    // 4 - Start CPU timer and wait up to SAMPLE_PERIOD_TIME
    TaskUtils::SleepMilli(RadDriver::kSamplePeriodTime);

    // 5 - Read Counter
    uint16_t counts = io_expander.RangeIoPinsToInt(I2cIoExpander::kIoPin0,
                                                   I2cIoExpander::kIoPin11);

    // 6 - Calculate radiation dosage
    double rads = (1.0 * counts) / RadDriver::kSamplePeriodTime;

    return rads;
}
