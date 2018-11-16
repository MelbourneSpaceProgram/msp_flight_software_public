#ifndef SRC_SENSORS_I2C_SENSORS_RTC_H_
#define SRC_SENSORS_I2C_SENSORS_RTC_H_

#include <external/etl/map.h>
#include <src/sensors/i2c_sensors/i2c_device.h>
#include <src/util/data_types.h>
#include <time.h>
#include <functional>

typedef etl::map<byte, byte, 6, std::less<uint8_t> > RTCMaskMap;

class Rtc : public I2cDevice {
   public:
    Rtc(const I2c* bus, int address, const I2cMultiplexer* multiplexer = NULL,
        I2cMultiplexer::MuxChannel channel = I2cMultiplexer::kMuxNoChannel);

    RTime GetTime();
    void SetSensorData(RTime reading);
    byte GetUnitTime(byte write_buffer, byte read_buffer[]);
    static bool ValidTime(RTime time);
    byte BCDToBinary(byte output);
    static time_t RTimeToEpoch(RTime time);

   private:
    static constexpr byte kTimeRegisterSec = 0x01;
    static constexpr byte kTimeRegisterMin = 0x02;
    static constexpr byte kTimeRegisterHour = 0x03;
    static constexpr byte kTimeRegisterDate = 0x04;
    static constexpr byte kTimeRegisterMonth = 0x05;
    static constexpr byte kTimeRegisterYear = 0x06;
    static constexpr byte kControl1Register = 0x10;
    static constexpr byte kControl2Register = 0x11;
    static constexpr byte kSQWRegister = 0x13;

    // bit_mask used to clear bits used by register for general purpose use
    static constexpr byte kLowest8BitMask = ~(0);
    static constexpr byte kLowest7BitMask = static_cast<byte>(~(0x01u << 7));
    static constexpr byte kLowest6BitMask = static_cast<byte>(~(0x03u << 6));
    static constexpr byte kLowest5BitMask = static_cast<byte>(~(0x07u << 5));
    static constexpr byte kHighest4BitMask = ~(15);
    static constexpr byte kLowest4BitMask = (15);

    // Pin pulsing frequency (100Hz) & the enable configuration
    static constexpr byte kSQWValue = 0x97;
    static constexpr byte kControl2ConfigValue = 0x3d;

    RTCMaskMap bit_mask_map;

    static constexpr uint8_t kReadBufLen = 1;

    enum Months {
        Jan = 0x01,
        Feb,
        Mar,
        Apr,
        May,
        Jun,
        Jul,
        Aug,
        Sep,
        Oct,
        Nov,
        Dec
    };
};

#endif  //  SRC_SENSORS_I2C_SENSORS_RTC_H_
