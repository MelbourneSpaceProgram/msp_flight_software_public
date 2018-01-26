#ifndef SRC_TELECOMMS_ANTENNA_H_
#define SRC_TELECOMMS_ANTENNA_H_

#include <src/debug_interface/debug_stream.h>
#include <src/i2c/i2c.h>
#include <src/i2c/i2c_configuration.h>
#include <src/messages/antenna_message.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Task.h>

// TODO(dingbenjamin): Change all i2c transactions to use timeout
class Antenna {
   public:
    static Antenna* GetAntenna();
    void InitAntenna(I2c* bus);  // Initial call is not thread safe
    // TODO(dingbenjamin): Use/investigate state machine framework
    void ForceDeploy() const;
    // TODO(dingbenjamin): Use/investigate state machine framework
    void SafeDeploy() const;
    AntennaMessage GetStatus() const;
    bool IsDoorsOpen() const;
    bool IsInitialised() const;
    I2c* GetBus() const;

   private:
    // I2C Address
    static const byte kAddress = 0x33;
    // Clear all recent commands
    static const byte kCommandModeAllOff = 0x00;
    // Auto Heating (Normal)
    static const byte kCommandModeAutoHeat = 0x01;
    // Auto Heating (Back up)
    static const byte kCommandModeAutoBackupHeat = 0x02;
    // Auto Heating (Combined)
    static const byte kCommandModeAutoCombinedHeat = 0x03;
    // Turn ON Heater 1 for 40 seconds
    static const byte kCommandModeHeaterOneOnDefTime = 0x04;
    // Turn ON Heater 2 for 40 seconds
    static const byte kCommandModeHeaterTwoOnDefTime = 0x05;
    // Turn ON Heater 1 + 2 for 40 seconds
    static const byte kCommandModeHeaterOneTwoOnDefTime = 0x06;
    // Go to SLEEP mode
    static const byte kCommandModeSleep = 0x07;
    // Door 1 bit mask in status packet
    static const byte kDoorOneMask = 0b10000000;
    // Door 2 bit mask in status packet
    static const byte kDoorTwoMask = 0b01000000;
    // Door 3 bit mask in status packet
    static const byte kDoorThreeMask = 0b00100000;
    // Door 4 bit mask in status packet
    static const byte kDoorFourMask = 0b00010000;
    // State bit mask in status packet
    static const byte kStateMask = 0b00000111;
    // Zero bit mask in status packet (Useful for endianess check)
    static const byte kZeroMask = 0b00001000;
    // Time to wait after trying a heater
    static const uint32_t kWaitTimeShort = 10000;
    // Time to wait after trying a heater
    static const uint32_t kWaitTimeLong = 20000;

    Antenna();
    // bool ValidBitRate(I2cConfiguration::bit_rates bit_rate) const;
    bool TryHeater(const byte heat_command, uint32_t wait_time) const;

    bool initialised;
    static Antenna* instance;
    I2c* bus;
};

#endif  // SRC_TELECOMMS_ANTENNA_H_
