#ifndef SRC_TELECOMMS_ANTENNA_H_
#define SRC_TELECOMMS_ANTENNA_H_

#include <src/board/i2c/io_expander/io_expander.h>
#include <src/util/data_types.h>

class I2c;
class AntennaMessage;

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
    void DeployAntenna();

   private:
    // I2C Address
    static constexpr byte kAddress = 0x33;
    // Door 1 bit mask in status packet
    static constexpr byte kDoorOneMask = 0b10000000;
    // Door 2 bit mask in status packet
    static constexpr byte kDoorTwoMask = 0b01000000;
    // Door 3 bit mask in status packet
    static constexpr byte kDoorThreeMask = 0b00100000;
    // Door 4 bit mask in status packet
    static constexpr byte kDoorFourMask = 0b00010000;
    // State bit mask in status packet
    static constexpr byte kStateMask = 0b00000011;
    // Time to wait (in milliseconds) after trying deployment algorithms
    static constexpr uint32_t kWaitTime = 10000;
    // Time to wait (in milliseconds) when manually overriding deployment
    static constexpr uint32_t kWaitTimeManualOverride = 10000;
    // TODO(wschuetz): Confirm with EnduroSat how long override should be held
    // high for
    static constexpr uint8_t kMaxNumberOfIterations = 0x03;
    static constexpr byte kAntennaOverRideIoExpanderAddress = 0x22;

    bool WriteCommand(AntennaCommand command) const;
    Antenna();

    bool initialised;
    static Antenna* instance;
    I2c* bus;
};

#endif  // SRC_TELECOMMS_ANTENNA_H_
