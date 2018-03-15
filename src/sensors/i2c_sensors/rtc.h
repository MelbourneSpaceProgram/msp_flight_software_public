#ifndef SRC_SENSORS_I2C_SENSORS_RTC_H_
#define SRC_SENSORS_I2C_SENSORS_RTC_H_

#include <external/etl/map.h>
#include <src/sensors/i2c_sensors/i2c_sensor.h>
#include <src/util/data_types.h>
#include <ti/sysbios/knl/Task.h>
#include <functional>

typedef struct time {
    byte sec;
    byte min;
    byte hour;
    byte date;
    byte month;
    uint16_t year;
} RTime;

typedef etl::map<byte, byte, 6, std::less<uint8_t> > RTCMaskMap;

class Rtc : public I2cSensor<RTime> {
   public:
    Rtc(I2c* bus, int address, std::string id);

    bool TakeReading(void);
    void SetSensorData(RTime reading);
    byte GetUnitTime(byte write_buffer, byte read_buffer[]);
    bool ValidTime(RTime time);
    byte BCDToBinary(byte output);

   private:
    static const byte kTimeRegisterSec = 0x01;
    static const byte kTimeRegisterMin = 0x02;
    static const byte kTimeRegisterHour = 0x03;
    static const byte kTimeRegisterDate = 0x04;
    static const byte kTimeRegisterMonth = 0x05;
    static const byte kTimeRegisterYear = 0x06;
    static const byte kControl1Register = 0x10;

    // bit_mask used to clear bits used by register for general purpose use
    static const byte kLowest8BitMask = ~(0);
    static const byte kLowest7BitMask = ~(1 << 7);
    static const byte kLowest6BitMask = ~(3 << 6);
    static const byte kLowest5BitMask = ~(7 << 5);
    static const byte kHighest4BitMask = ~(15);
    static const byte kLowest4BitMask = (15);

    RTCMaskMap bit_mask_map;

    static const uint8_t kReadBufLen = 1;
    static const uint16_t kCurrCentury = 2000;

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
