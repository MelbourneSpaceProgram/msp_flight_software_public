/**
  tca9548a.h
  Purpose: Controls a TCA9548A multiplexer.

  @author Brett Clark
  @version 1.0
  @date 3/6/2017
*/

#ifndef _TCA9548A_H_
#define _TCA9548A_H_

#include "i2c_multiplexer.h"
#include "i2c_sensor.h"
#include "string"
#include "iostream"


using namespace std;

/**
  TCA9548A class.

  This describes a single TCA9548A multiplexer.
*/
class TCA9548A : public I2CMultiplexer {

/**
  Public members of the TCA9548A class.
*/
public:

  /**
    TCA9548A constructor.

    @param bus The bus that the multiplexer is connected to.
    @param address The address of the multiplexer.
    @return The TCA9548A object.
  */
  TCA9548A(I2CBus* bus, int address);

  /**
    Method that opens a channel of the multiplexer.
    This is an implementation of the virtual function in I2CMultiplexer.

    @param channel The channel to open.
  */
  void open_channel(int channel);
  void close_all_channels();
  uint8_t* get_channel_states();

  /**
    Public constants of the TCA9548A class.
  */

  /**
    The channels of the TCA9548A multiplexer.
  */
  static const int CHANNEL_0;
  static const int CHANNEL_1;
  static const int CHANNEL_2;
  static const int CHANNEL_3;
  static const int CHANNEL_4;
  static const int CHANNEL_5;
  static const int CHANNEL_6;
  static const int CHANNEL_7;

/**
  Private members of the TCA9548A class.
*/
private:

  /**
    The register in which the TMP006 die temperature is stored.
  */
  static const int DIE_TEMP_REGISTER;

  /**
    Member variables of the TCA9548A class.
  */

  /**
    A reference to the bus that the multiplexer is connected to.
  */
  I2CBus* bus;

  /**
    The address of the multiplexer.
  */
  int address;
};

#endif // _TCA9548A_H_
