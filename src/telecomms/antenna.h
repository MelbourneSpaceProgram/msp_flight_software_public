#ifndef SRC_TELECOMMS_ANTENNA_H_
#define SRC_TELECOMMS_ANTENNA_H_

#include <src/i2c/i2c.h>
#include <src/messages/antenna_message.h>
#include <src/util/data_types.h>

class Antenna {
   public:
    enum AntennaCommand {
        kCommandModeAllOff = 0x00,
        kCommandAllDoorsAlgorithm1 = 0x1F,
        kCommandAllDoorsAlgorithm2 = 0x2F
    };
    static Antenna* GetAntenna();
    void InitAntenna(I2c* bus);  // Initial call is not thread safe
    // TODO(dingbenjamin): Use/investigate state machine framework
    bool SafeDeploy() const;
    bool TryAlgorithm(AntennaCommand command) const;
    bool ForceDeploy() const;
    AntennaMessage GetStatus() const;
    bool IsDoorsOpen() const;
    bool IsHeatersOn() const;
    bool IsInitialised() const;
    I2c* GetBus() const;

   private:
    // I2C Address
    static const byte kAddress = 0x33;
    // Door 1 bit mask in status packet
    static const byte kDoorOneMask = 0b10000000;
    // Door 2 bit mask in status packet
    static const byte kDoorTwoMask = 0b01000000;
    // Door 3 bit mask in status packet
    static const byte kDoorThreeMask = 0b00100000;
    // Door 4 bit mask in status packet
    static const byte kDoorFourMask = 0b00010000;
    // State bit mask in status packet
    static const byte kStateMask = 0b00000011;
    // Time to wait (in milliseconds) after trying deployment algorithms
    static const uint32_t kWaitTime = 20000;
    // Time to wait (in milliseconds) when manually overriding deployment
    static const uint32_t kWaitTimeManualOverride = 10000;
    // TODO(wschuetz): Confirm with EnduroSat how long override should be held
    // high for
    static const uint8_t kPrimaryOverridePin = 0x01;
    static const uint8_t kBackupOverridePin = 0x02;
    static const uint8_t kMaxNumberOfIterations = 0x03;

    bool WriteCommand(AntennaCommand command) const;
    Antenna();

    bool initialised;
    static Antenna* instance;
    I2c* bus;
};

#endif  // SRC_TELECOMMS_ANTENNA_H_
